//
//  TCPClient.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/14.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPClient.hpp"
#include "TCPSession.hpp"
#include "TCPConnector.hpp"
#include "DataBuffer.hpp"

TCPClient::TCPClient(asio::io_context& io_context,
          const std::string& ip,
          unsigned short port)
: io_context_(io_context)
, connector_(new TCPConnector(io_context, ip, port))
{
    connector_->setNewConnectionCallback(std::bind(&TCPClient::newConnection, this, std::placeholders::_1));
}

void TCPClient::connect()
{
    connector_->start();
}
void TCPClient::disconnect()
{
    if (session_) {
        session_->shutdown();
    }
}

void TCPClient::stop()
{
    if (connector_) {
        connector_->stop();
    }
}

void TCPClient::newConnection(std::shared_ptr<tcp::socket> socket)
{
    auto endpoint = socket->remote_endpoint();
    endpoint.port();
    
    char buf[64];
    snprintf(buf, sizeof(buf), "%s:%d", endpoint.address().to_string().c_str(), endpoint.port());
    
    std::string name(buf);
    std::shared_ptr<TCPSession> session(new TCPSession(socket, name)); 
    if (writeCompleteCallback_) {
        session->setWriteCompleteCallback(writeCompleteCallback_);
    } else {
        session->setWriteCompleteCallback(std::bind(&TCPClient::defaultWriteCompleteCallback, this, std::placeholders::_1));
    }
    if (messageCallback_) {
        session->setMessageCallback(messageCallback_);
    } else {
        session->setMessageCallback(std::bind(&TCPClient::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2));
    }
    if (connectionCallback_){
        session->setConnectionCallback(connectionCallback_);
    } else {
        session->setConnectionCallback(std::bind(&TCPClient::defaultConnectionCallback, this, std::placeholders::_1));
    }
    session->setCloseCallback(std::bind(&TCPClient::removeConnection, this, std::placeholders::_1));
    session->connectEstablished();
    session_ = session;
}

void TCPClient::defaultMessageCallback(const std::shared_ptr<TCPSession>&, DataBuffer*const)
{
    
}

void TCPClient::defaultWriteCompleteCallback(const std::shared_ptr<TCPSession>&)
{
    
}

void TCPClient::defaultConnectionCallback(const std::shared_ptr<TCPSession>&)
{
    
}

void TCPClient::removeConnection(const std::shared_ptr<TCPSession>&)
{
//    session_.reset();
    session_->connectDestroyed();
}







