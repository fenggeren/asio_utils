//
//  TCPConnector.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

//#include "asio.hpp"
#include "Header.h"

namespace fasio
{

class TCPConnector
{
    using SocketPtr = std::shared_ptr<asio::ip::tcp::socket>;
public:
    
    using NewConnectionCallback = std::function<void(SocketPtr)>;
    
    TCPConnector(asio::io_context& ictx,
                 const std::string& address,
                 unsigned short port);
    
    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        newConnectionCallback_ = cb;
    }
    
    void setRetryNum(int num) { retryNum_ = num; }
    
    void start();
    
    void restart();
    
    // 停止连接
    void stop();
    
    asio::ip::tcp::endpoint endpoint() const { return endpoint_; }
    
private:
    
    enum States { kDisconnected, kConnecting, kConnected, kStopped };

    
    void setState(States s) { state_ = s; }
    
    void handleConnect(SocketPtr socket, std::error_code ec);
    
    void retry(SocketPtr socket);
    
    void resolveAddress();
    
    void connect();
private:
    asio::io_context& io_context_;
    asio::ip::tcp::endpoint endpoint_;
    NewConnectionCallback newConnectionCallback_;
    
    asio::basic_waitable_timer<std::chrono::steady_clock> retryTimer_;
    
    bool connect_;
    States state_;
    int retryDelayMs_;
    int retryNum_;
    
    const std::string address_;
    const unsigned short port_;
    
    asio::ip::tcp::resolver::iterator endpoint_iterator_;
    asio::ip::tcp::resolver resolve_;
};

}
