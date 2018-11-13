//
//  ClientSession.hpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <CPG/Net/CPGNetSession.hpp>
#include <Net/TCPSessionFactory.h>
#include <CPG/CPGServerDefine.h>
 
using namespace fasio;

class CGSession : public CPGServerSession
{
public:
    CGSession(SocketPtr socket, const std::string& name):
    CPGServerSession(socket, name)
    {
    }
    
private:
    
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
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
        // 可以使用 IOContextPool 返回， 但是一些数据要做多线程同步处理.
        return io_context_;
    }
    virtual uint8 type()
    {
        return ServerType_Client;
    }
    
private:
    asio::io_context& io_context_;
};

