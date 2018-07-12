//
//  TCPConnector.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include "asio.hpp"


class TCPConnector
{
public:

    using NewConnectionCallback = std::function<void(std::shared_ptr<asio::ip::tcp::socket>)>;
    
    TCPConnector(asio::io_context& ictx,
                 const std::string& ip,
                 unsigned short port);
    
    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        newConnectionCallback_ = cb;
    }
    
    void start();
    
    void restart();
    
    void stop();
    
    asio::ip::tcp::endpoint endpoint() const { return endpoint_; }
    
private:
    
    enum States { kDisconnected, kConnecting, kConnected };
    static const int kMaxRetryDelayMs = 30*1000;
    static const int kInitRetryDelayMs = 500;
    
    void setState(States s) { state_ = s; }
    
    void connect();
    
    void retry(const asio::ip::tcp::socket& socket);
    
    
private:
    asio::io_context& io_context_;
    asio::ip::tcp::endpoint endpoint_;
    NewConnectionCallback newConnectionCallback_;
    bool connect_;
    States state_;
    int retryDelayMs_;
};

