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

TCPServer::TCPServer(asio::io_context& io, unsigned short port)
: io_context_(io)
, acceptor_(new TCPAcceptor(io))
{
    acceptor_->setNewConnectionCallback(std::bind(&TCPServer::newConnection, this, std::placeholders::_1));
    
}


void TCPServer::newConnection(std::shared_ptr<tcp::socket> socket)
{
    TCPSession session(socket);
    session.connectEstablished();
    
}
