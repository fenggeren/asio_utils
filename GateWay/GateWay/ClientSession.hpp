//
//  ClientSession.hpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <Net/TCPSession.hpp>
#include <Net/TCPSessionFactory.h>
#include <CPG/CPGServerDefine.h>
 
using namespace fasio;

class CGSession : public TCPSession
{
public:
    CGSession(SocketPtr socket, const std::string& name):
    TCPSession(socket, name)
    {
        messageCallback_ = std::bind(&CGSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        
    }
    
private:
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data);
private:
    
    void loginRQ(const void* data, int len);
    
};

class CGSessionFactory : public TCPSessionFactory
{
public:
    
    CGSessionFactory(asio::io_context& io):
    io_context_(io)
    {}
    
    virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket> sock)
    {
        auto endpoint = sock->remote_endpoint();
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "CG-%s:%d",
                 endpoint.address().to_string().data(), endpoint.port());
        
        std::string name(buf);
        std::shared_ptr<CGSession> session(new CGSession(sock, name));
        
        return session;
    }
    virtual asio::io_context& io_context()
    {
        return io_context_;
    }
    virtual uint8 type()
    {
        return ServerType_Client;
    }
    
private:
    asio::io_context& io_context_;
};

