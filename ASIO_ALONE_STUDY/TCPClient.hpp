//
//  TCPClient.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/14.
//  Copyright © 2018年 fenggeren. All rights reserved.
//


#pragma once

#include <memory>
#include "Header.h"

class TCPConnector;
class TCPSession;
class DataBuffer;

class TCPClient
{
public:
    
    TCPClient(asio::io_context& io_context,
              const std::string& ip,
              unsigned short port);
    
    void connect();
    void disconnect();
    void stop();
    
    
    
    void setConnectionCallback(const std::function<void(const std::shared_ptr<TCPSession>&)> cb)
    {
        connectionCallback_ = cb;
    }
    void setWriteCompleteCallback(const std::function<void(const std::shared_ptr<TCPSession>&)>& cb)
    {
        writeCompleteCallback_ = cb;
    }
    void setMessageCallback(const std::function<void(const std::shared_ptr<TCPSession>&, DataBuffer*const)>& cb)
    {
        messageCallback_ = cb;
    }
    
private:
    
    void newConnection(std::shared_ptr<asio::ip::tcp::socket> socket);
    void defaultMessageCallback(const std::shared_ptr<TCPSession>&, DataBuffer*const);
    void defaultWriteCompleteCallback(const std::shared_ptr<TCPSession>&);
    void defaultConnectionCallback(const std::shared_ptr<TCPSession>&);
    void removeConnection(const std::shared_ptr<TCPSession>&);
private:
    
    asio::io_context& io_context_;
    std::shared_ptr<TCPConnector> connector_;
    std::shared_ptr<TCPSession> session_;
    
    std::function<void(const std::shared_ptr<TCPSession>&)> connectionCallback_;
    std::function<void(const std::shared_ptr<TCPSession>&)> writeCompleteCallback_;
    std::function<void(const std::shared_ptr<TCPSession>&, DataBuffer*const)> messageCallback_;
};












