//
//  TCPAcceptor.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPAcceptor.hpp"
#include <iostream>
#include "Logging.hpp"
using namespace fasio::logging;

namespace fasio
{


TCPAcceptor::TCPAcceptor(asio::io_context& io_context)
: io_context_(io_context)
, acceptor_(io_context_)
{
    
}

void TCPAcceptor::listen(unsigned short port, bool ipv6)
{
    
    try
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
    catch (const std::exception& ec)
    {
        LOG_ERROR << "cannot bind/listen on port." << port
        << " err: " << ec.what();
    }
}

void TCPAcceptor::listenInternal()
{
    std::shared_ptr<tcp::socket> new_socket(new tcp::socket(io_context_));
    acceptor_.async_accept(*new_socket, std::bind(&TCPAcceptor::handAccept, this,new_socket, std::placeholders::_1));
}

void TCPAcceptor::handAccept(std::shared_ptr<tcp::socket> socket, std::error_code ec)
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












}
