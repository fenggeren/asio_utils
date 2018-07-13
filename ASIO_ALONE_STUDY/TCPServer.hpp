//
//  TCPServer.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//


#pragma once


#include "asio.hpp"


class TCPAcceptor;

using asio::ip::tcp;

class TCPServer
{
public:
    
    TCPServer(asio::io_context& io, unsigned short port);
    
    
private:
    
    void newConnection(std::shared_ptr<tcp::socket> socket);
    
private:
    
    std::shared_ptr<TCPAcceptor> acceptor_;
    asio::io_context& io_context_;
};
















