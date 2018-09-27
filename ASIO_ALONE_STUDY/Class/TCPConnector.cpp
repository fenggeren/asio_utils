//
//  TCPConnector.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPConnector.hpp"
#include <iostream>

static const int kMaxRetryDelayMs = 30000;
static const int kInitRetryDelayMs = 500;

TCPConnector::TCPConnector(asio::io_context& ictx,
             const std::string& address,
             unsigned short port)
: io_context_(ictx)
, resolve_(ictx)
, retryTimer_(io_context_)
, address_(address)
, port_(port)
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
    if (state_ == kConnecting) {
        return;
    }
    
    connect_ = true;
    setState(kConnecting);
    resolveAddress();
}

void TCPConnector::connect()
{
    SocketPtr
    socketPtr(new asio::ip::tcp::socket(io_context_));
    socketPtr->async_connect(*endpoint_iterator_,std::bind(&TCPConnector::handleConnect, this, socketPtr, std::placeholders::_1));
}

void TCPConnector::handleConnect(SocketPtr socket, std::error_code ec)
{
    if (!ec)
    {
        setState(kConnected);
        if (newConnectionCallback_)
        {
            newConnectionCallback_(socket);
        }
    }
    else if (++endpoint_iterator_ != asio::ip::tcp::resolver::iterator())
    {
        connect();
    }
    else if (ec.value() == asio::error::try_again ||
             ec.value() == asio::error::address_in_use ||
             ec.value() == asio::error::no_protocol_option ||
             ec.value() == asio::error::connection_refused ||
             ec.value() == asio::error::host_unreachable)
    {
        retry(socket);
    }
    else if (ec.value() == asio::error::no_permission ||
             ec.value() == asio::error::access_denied ||
             ec.value() == asio::error::address_family_not_supported ||
             ec.value() == asio::error::already_started ||
             ec.value() == asio::error::bad_descriptor ||
             ec.value() == asio::error::fault ||
             ec.value() == asio::error::not_socket)
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
        retryTimer_.async_wait([this](std::error_code)
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


void TCPConnector::resolveAddress()
{
    resolve_.async_resolve(address_,
                          std::to_string(port_),
                          [&](std::error_code ec,
                             asio::ip::basic_resolver_results<asio::ip::tcp> results)
                          {
                              endpoint_iterator_ = results.begin();
                              connect();
                          });
}







