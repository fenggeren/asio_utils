//
//  TestSessionManager.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include "TCPSessionManager.hpp"
#include "TCPSessionFactory.h"
#include "TCPSession.hpp"
#include "NetPacket.hpp"
#include "DataBuffer.hpp"

using namespace fasio;
namespace TESTSM
{
static asio::io_context g_IoContext;

enum ServerType
{
    ServerType_CG = 0,
    ServerType_GL,
    ServerType_GM,
    ServerType_GC
};

class GSSession : public TCPSession
{
public:
    GSSession(SocketPtr socket, const std::string& name):
    TCPSession(socket, name)
    {
        messageCallback_ = std::bind(&GSSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        
    }
    
private:
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data)
    {
        std::string content(data->peek(), data->readableBytes());
        data->retrieveAll();
        session->send(content);
    }
private:
};

class GSSessionFactory : public TCPSessionFactory
{
public:
    virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket> sock)
    {
         auto endpoint = sock->remote_endpoint();
         char buf[64] = {0};
         snprintf(buf, sizeof(buf), "CG-%s:%d",
                  endpoint.address().to_string().data(), endpoint.port());
        
        std::string name(buf);
        std::shared_ptr<GSSession> session(new GSSession(sock, name));
        
        return session;
    }
    virtual asio::io_context& io_context()
    {
        return g_IoContext;
    }
    virtual uint8 type()
    {
        return ServerType_CG;
    }
};



class TestSessionManager : public TCPSessionManager
{
public:
    
    void start()
    {
        auto factory = std::make_shared<GSSessionFactory>();
        createListener(7890, false, factory);
        createListener(7891, false, factory);
        createListener(7892, false, factory);
        
        g_IoContext.run();
    }
    
private:

};





}










