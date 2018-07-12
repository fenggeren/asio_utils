//
//  TCPConnector.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPConnector.hpp"

static const int kMaxRetryDelayMs = 30000;
static const int kInitRetryDelayMs = 500;

TCPConnector::TCPConnector(asio::io_context& ictx,
             const std::string& ip,
             unsigned short port)
: endpoint_(asio::ip::address_v4::from_string(ip), port)
, io_context_(ictx)
, retryTimer_(io_context_)
, connect_(false)
, state_(kDisconnected)
, retryDelayMs_(kInitRetryDelayMs)
{
    
}


void TCPConnector::restart()
{
    setState(kDisconnected);
    retryDelayMs_ = kInitRetryDelayMs;
    connect_ = true;
    start();
}

void TCPConnector::stop()
{
    if (state_ == kConnecting)
    {
        setState(kStopped);
    }
}

void TCPConnector::start()
{
    connect_ = true;
    setState(kConnecting);
    
    SocketPtr
    socketPtr(new asio::ip::tcp::socket(io_context_));
    
    socketPtr->async_connect(endpoint_,std::bind(&TCPConnector::handleConnect, this, socketPtr, std::placeholders::_1));
}

void TCPConnector::handleConnect(SocketPtr socket, asio::error_code ec)
{
    if (!ec)
    {
        setState(kConnected);
        if (newConnectionCallback_)
        {
            newConnectionCallback_(socket);
        }
    }
    else if (ec == asio::error::try_again ||
             ec == asio::error::address_in_use ||
             ec == asio::error::no_protocol_option ||
             ec == asio::error::connection_refused ||
             ec == asio::error::host_unreachable)
    {
        retry(socket);
    }
    else if (ec == asio::error::no_permission ||
             ec == asio::error::access_denied ||
             ec == asio::error::address_family_not_supported ||
             ec == asio::error::already_started ||
             ec == asio::error::bad_descriptor ||
             ec == asio::error::fault ||
             ec == asio::error::not_socket)
    {
        socket->close();
        std::cout << "connect error in TCPConnector " << ec << std::endl;
    }
    else
    {
        socket->close();
    }
}


void TCPConnector::retry(SocketPtr socket)
{
    socket->close();
    socket.reset();
    if (state_ == kStopped)
    {
        std::cout << " TCPConnector retry 已经停止链接" << std::endl;
        return;
    }
    setState(kDisconnected);
    
    if (connect_)
    {
        retryTimer_.expires_after(std::chrono::milliseconds(retryDelayMs_));
        retryTimer_.async_wait([this](asio::error_code)
        {
            start();
        });
        retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
        std::cout << " retry delay ms_ " << retryDelayMs_ << std::endl;
    }
    else
    {
        std::cout << "do not connect" << std::endl;
    }
}










