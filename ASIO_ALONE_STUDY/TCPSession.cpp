//
//  TCPSession.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/10.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPSession.hpp"

TCPSession::TCPSession(SocketPtr socket)
: socket_(std::move(socket)),
sendBuffer_(new DataBuffer),
outputBuffer_(new DataBuffer),
inputBuffer_(new DataBuffer)
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
    socket_->async_read_some(asio::buffer(inputBuffer_->beginWrite(),
                                         inputBuffer_->writableBytes()),
                            std::bind(&TCPSession::handRead,this,
                                      std::placeholders::_1,
                                      std::placeholders::_2));
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
    if (sendBuffer_->isOperate())
    {
        return;
    }
    std::swap(sendBuffer_, outputBuffer_);
    
    socket_->async_send(asio::buffer(sendBuffer_->beginWrite(),
                                    sendBuffer_->writableBytes()),
                       std::bind(&TCPSession::handWrite,this,
                                 std::placeholders::_1,
                                 std::placeholders::_2));
    sendBuffer_->operate();

}

void TCPSession::handRead(asio::error_code ec, std::size_t bytesRead)
{
    if (ec != asio::error::operation_aborted)
    {
        if (!ec)
        {
            inputBuffer_->hasWritten(bytesRead);
            messageCallback_(shared_from_this(), inputBuffer_);
            startAsyncRead();
        }
        else
        {
            // TODO. 处理错误
        }
    }
}

void TCPSession::handWrite(asio::error_code ec, std::size_t bytesRead)
{
    sendBuffer_->unoperate();
    sendBuffer_->retrieveAll();
    if (writeCompleteCallback_) {
        writeCompleteCallback_(shared_from_this());
    }
    internalSend();
}

void TCPSession::handClose()
{
    if (connected())
    {
        socket_->close();
        socket_ = nullptr;
        closeCallback_(shared_from_this());
    }
}


void TCPSession::connectEstablished()
{
    if (connectionCallback_) {
        connectionCallback_(shared_from_this());
    }
}

void TCPSession::connectDestroyed()
{
    forceClose();
}

void TCPSession::shutdown()
{
    if (socket_ && socket_->is_open())
    {
        
        socket_->shutdown(asio::socket_base::shutdown_send);
    }
}
void TCPSession::forceClose()
{
    if (socket_ && socket_->is_open())
    {
        handClose();
    }
}





