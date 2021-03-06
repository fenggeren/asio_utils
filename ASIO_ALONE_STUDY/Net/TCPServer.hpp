//
//  TCPServer.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//


#pragma once


#include "Header.h"
#include <unordered_map>

namespace fasio
{


class TCPAcceptor;
class TCPSession;
class DataBuffer;

using asio::ip::tcp;

class TCPServer
{
public:
    
    TCPServer(asio::io_context& io, unsigned short port, bool ipv6=false);
    
    void start();
    
    void setMessageCallback(const std::function<void(const std::shared_ptr<TCPSession>&, DataBuffer*const)>& callback)
    {
        messageCallback_ = callback;
    }
    
    void setConnectionCallback(const std::function<void(const std::shared_ptr<TCPSession>&)>& callback)
    {
        connectionCallback_ = callback;
    }
    void setRemoveConnectionCallback(const std::function<void(const std::shared_ptr<TCPSession>&)>& callback)
    {
        removeConnectionCallback_ = callback;
    }
private:
    
    void newConnection(std::shared_ptr<tcp::socket> socket);
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>&, DataBuffer*const);
    void defaultWriteCompleteCallback(const std::shared_ptr<TCPSession>&);
    void defaultConnectionCallback(const std::shared_ptr<TCPSession>&);
    void removeConnection(const std::shared_ptr<TCPSession>&);
private:
    
    std::shared_ptr<TCPAcceptor> acceptor_;
    asio::io_context& io_context_;
    
    std::unordered_map<std::string, std::shared_ptr<TCPSession>> connectionsMap_;
    
    int nextConnectionID_{0};
    
    std::function<void(const std::shared_ptr<TCPSession>&)> connectionCallback_;
    std::function<void(const std::shared_ptr<TCPSession>&)> writeCompleteCallback_;
    std::function<void(const std::shared_ptr<TCPSession>&, DataBuffer*const)> messageCallback_;
    std::function<void(const std::shared_ptr<TCPSession>&)> removeConnectionCallback_;
    unsigned short port_;
    bool ipv6_;
};
















}
