//
//  TCPAcceptor.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include "Header.h"

using asio::ip::tcp;

class TCPAcceptor
{
    using NewConnectionCallback = std::function<void(std::shared_ptr<tcp::socket>)>;
public:
    
    TCPAcceptor(asio::io_context& io_context);
    
    void listen(unsigned short port, bool ipv6);
    
    
    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }
    
private:
    
    void listenInternal();
    void handAccept(std::shared_ptr<tcp::socket> socket, std::error_code ec);
    
private:
    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    NewConnectionCallback newConnectionCallback_;
};

