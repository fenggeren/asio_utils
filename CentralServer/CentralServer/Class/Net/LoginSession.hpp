//
//  LoginSession.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//
#pragma once

#include <CPG/Net/CPGNetSession.hpp> 
#include <Net/TCPSessionFactory.h>
#include <CPG/CPGServerDefine.h>


using namespace fasio;

class LoginSession : public CPGServerSession
{
public:
    LoginSession(SocketPtr socket, const std::string& name):
    CPGServerSession(socket, name)
    { 
    }
public:
    
    virtual void onClose() override;
private:
    
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header) override;
private:
};

class LoginSessionFactory : public TCPSessionFactory
{
public:
    
    LoginSessionFactory(asio::io_context& io):
    io_context_(io)
    {}
    
    virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket> sock)
    {
        auto endpoint = sock->remote_endpoint();
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "LC-%s:%d",
                 endpoint.address().to_string().data(), endpoint.port());
        
        std::string name(buf);
        std::shared_ptr<LoginSession> session(new LoginSession(sock, name));
        
        return session;
    }
    virtual asio::io_context& io_context()
    {
        return io_context_;
    }
    virtual uint8 type()
    {
        return ServerType_LoginServer;
    }
    
private:
    asio::io_context& io_context_;
};

