//
//  TCPAcceptor.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPAcceptor.hpp"


TCPAcceptor::TCPAcceptor(asio::io_context& io_context)
: io_context_(io_context)
, acceptor_(io_context_)
{
    
}

void TCPAcceptor::listen(unsigned short port, bool ipv6)
{
    tcp::endpoint endpoint(ipv6 ? tcp::v6() : tcp::v4(),
                           port);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(tcp::acceptor::reuse_address(true));
    if (ipv6) {
        acceptor_.set_option(asio::ip::v6_only(false));
    }
    acceptor_.bind(endpoint);
    acceptor_.listen();
 
    listenInternal();
}

void TCPAcceptor::listenInternal()
{
    std::shared_ptr<tcp::socket> new_socket(new tcp::socket(io_context_));
    acceptor_.async_accept(*new_socket, std::bind(&TCPAcceptor::handAccept, this,new_socket, std::placeholders::_1));
}

void TCPAcceptor::handAccept(std::shared_ptr<tcp::socket> socket, asio::error_code ec)
{
    if (!ec)
    {
        if (newConnectionCallback_)
        {
            newConnectionCallback_(socket);
            listenInternal();
        }
        else
        {
            socket->close();
        }
    }
    else
    {
        std::cout << "TCPAcceptor::handAccept ERROR " << ec;
    }
}











