//
//  GateSession.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <CPG/Net/CPGNetSession.hpp>
#include <Net/TCPSessionFactory.h>
#include <CPG/CPGServerDefine.h>


using namespace fasio;

class GateSession : public CPGServerSession
{
public:
    GateSession(SocketPtr socket, const std::string& name):
    CPGServerSession(socket, name)
    {
    }
    
private:
    
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
};

class GateSessionFactory : public TCPSessionFactory
{
public:
    
    GateSessionFactory(asio::io_context& io):
    io_context_(io)
    {}
    
    virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket> sock)
    {
        auto endpoint = sock->remote_endpoint();
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "GM-%s:%d",
                 endpoint.address().to_string().data(), endpoint.port());
        
        std::string name(buf);
        std::shared_ptr<GateSession> session(new GateSession(sock, name));
        
        return session;
    }
    virtual asio::io_context& io_context()
    {
        return io_context_;
    }
    virtual uint8 type()
    {
        return ServerType_GateServer;
    }
private:
    asio::io_context& io_context_;
};
