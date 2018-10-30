//
//  TCPSession.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/10.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPSession.hpp"
#include <iostream>
#include "Logging.hpp"
#include "TCPConnector.hpp"

using namespace fasio::logging;

namespace fasio
{

TCPSession::TCPSession(SocketPtr socket, const std::string& name)
: socket_(std::move(socket)),
name_(name),
sendBuffer_(new DataBuffer),
outputBuffer_(new DataBuffer),
inputBuffer_(new DataBuffer),
state_(kConnecting)
{
    
}

TCPSession::~TCPSession()
{
    delete sendBuffer_;
    delete outputBuffer_;
    delete inputBuffer_;
}

void TCPSession::startAsyncRead()
{
    if (state_ == kConnected)
    {
        socket_->async_read_some(asio::buffer(inputBuffer_->beginWrite(),
                                              inputBuffer_->writableBytes()),
                                 std::bind(&TCPSession::handRead,this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    }
}

void TCPSession::send(const std::string& message)
{
    send(message.data(), message.size());
}


void TCPSession::send(const void* message, size_t len)
{
    outputBuffer_->append(message, len);
    
    internalSend();
}

void TCPSession::internalSend()
{
    if (state_ != kConnected ||
        sendBuffer_->isOperate() ||
        outputBuffer_->readableBytes() == 0)
    {
        return;
    }
    std::swap(sendBuffer_, outputBuffer_);
    
    socket_->async_send(asio::buffer(sendBuffer_->beginRead(),
                                    sendBuffer_->readableBytes()),
                       std::bind(&TCPSession::handWrite,this,
                                 std::placeholders::_1,
                                 std::placeholders::_2));
    sendBuffer_->operate();

}

void TCPSession::handRead(std::error_code ec, std::size_t bytesRead)
{
    if (ec.value() != asio::error::operation_aborted)
    {
        try
        {
            if (!ec)
            {
                inputBuffer_->hasWritten(bytesRead);
                messageCallback_(shared_from_this(), inputBuffer_);
                startAsyncRead();
            }
            else
            {
                if (ec.value() == asio::error::interrupted ||
                    ec.value() == asio::error::try_again)
                {
                    startAsyncRead();
                }
                else
                {
                    LOG_MINFO << " connection closed: " << ec.value();
                    handClose();
                }
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR << " unhandled exception in handleread: " << ec.value();
            throw;
        }
    }
}

void TCPSession::handWrite(std::error_code ec, std::size_t bytesRead)
{
    if (!ec)
    {
        sendBuffer_->unoperate();
        sendBuffer_->retrieveAll();
        if (writeCompleteCallback_) {
            writeCompleteCallback_(shared_from_this());
        }
        internalSend();
    }
}

void TCPSession::handClose()
{
    state_ = kDisconnected;
    if (connected())
    {
        socket_->close();
        socket_ = nullptr;
        if (closeCallback_) {
            closeCallback_(shared_from_this());
        }
    }
}


void TCPSession::connectEstablished()
{
    state_ = kConnected;
    startAsyncRead();
    if (connectionCallback_) {
        connectionCallback_(shared_from_this());
    }
}

void TCPSession::connectDestroyed()
{
    if (state_ == kConnected)
    {
        state_ = kDisconnected;
        forceClose();
    }
}

void TCPSession::shutdown()
{
    if (state_ == kConnected)
    {
        state_ = kDisconnecting;
        socket_->shutdown(asio::socket_base::shutdown_send);
    }
}
void TCPSession::forceClose()
{
    if (state_ == kConnected || state_ == kDisconnecting)
    {
        state_ = kDisconnecting;
        handClose();
    }
}

void ClientSession::setConnector(std::shared_ptr<TCPConnector> connector)
{
    connector_ = connector;
    connector->setNewConnectionCallback(nullptr);
}

void ClientSession::reconnect()
{
    if (retry_)
    {
        connector_->restart();
    }
}
 
void ClientSession::stop()
{
    if (connector_) {
        connector_->stop();
    }
}



}
