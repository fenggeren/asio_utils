//
//  TCPServer.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPServer.hpp"
#include "TCPAcceptor.hpp"
#include "TCPSession.hpp"
#include <iostream>

TCPServer::TCPServer(asio::io_context& io, unsigned short port,bool ipv6)
: io_context_(io)
, acceptor_(new TCPAcceptor(io))
, port_(port)
, ipv6_(ipv6)
{
    acceptor_->setNewConnectionCallback(std::bind(&TCPServer::newConnection, this, std::placeholders::_1));
}

void TCPServer::start()
{
    acceptor_->listen(port_, ipv6_);
}

void TCPServer::newConnection(std::shared_ptr<tcp::socket> socket)
{
    auto endpoint = socket->remote_endpoint();
    endpoint.port();
    
    char buf[64];
    snprintf(buf, sizeof(buf), "%s:%d-%d", endpoint.address().to_string().c_str(), endpoint.port(), ++nextConnectionID_);
    
    std::string name(buf);
    std::shared_ptr<TCPSession> session(new TCPSession(socket, name));
    connectionsMap_[name] = session;
    if (writeCompleteCallback_) {
        session->setWriteCompleteCallback(writeCompleteCallback_);
    } else {
        session->setWriteCompleteCallback(std::bind(&TCPServer::defaultWriteCompleteCallback, this, std::placeholders::_1));
    }
    if (messageCallback_) {
        session->setMessageCallback(messageCallback_);
    } else {
        session->setMessageCallback(std::bind(&TCPServer::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2));
    }
    if (connectionCallback_){
        session->setConnectionCallback(connectionCallback_);
    } else {
        session->setConnectionCallback(std::bind(&TCPServer::defaultConnectionCallback, this, std::placeholders::_1));
    }
    session->setCloseCallback(std::bind(&TCPServer::removeConnection, this, std::placeholders::_1));
    session->connectEstablished();
}



void TCPServer::defaultMessageCallback(const TCPSessionPtr& session,
                                       DataBuffer* const buffer)
{
    std::string receive(buffer->beginRead(), buffer->readableBytes());
    buffer->retrieveAll();
    session->send(receive);
}

void TCPServer::defaultWriteCompleteCallback(const TCPSessionPtr& session)
{
//    std::cout << " write complete " << session->name() << std::endl;
}

void TCPServer::removeConnection(const std::shared_ptr<TCPSession>& session)
{
//    std::cout << " remove connection: " << session->name() << std::endl;
    connectionsMap_.erase(session->name());
}

void TCPServer::defaultConnectionCallback(const std::shared_ptr<TCPSession>& session)
{
//    std::cout << " connection: " << session->name() << std::endl;
}









