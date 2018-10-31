//
//  CSSessionManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>

using namespace fasio;

static asio::io_context g_IoContext;

enum ServerType
{
    ServerType_CG = 0,
    ServerType_Client_Gate = 1,
    ServerType_Gate_Login,
    ServerType_Gate_Match,
    ServerType_Gate_Central
};




class GateSession : public TCPSession
{
public:
    GateSession(SocketPtr socket, const std::string& name):
    TCPSession(socket, name)
    {
        messageCallback_ = std::bind(&GateSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        
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

class GateSessionFactory : public TCPSessionFactory
{
public:
    virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket> sock)
    {
        auto endpoint = sock->remote_endpoint();
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "CG-%s:%d",
                 endpoint.address().to_string().data(), endpoint.port());
        
        std::string name(buf);
        std::shared_ptr<GateSession> session(new GateSession(sock, name));
        
        return session;
    }
    virtual asio::io_context& io_context()
    {
        return g_IoContext;
    }
    virtual uint8 type()
    {
        return ServerType_Gate_Central;
    }
};



class CSSessionManager : public TCPSessionManager
{
public:
    
    void start()
    {
        auto factory = std::make_shared<GateSessionFactory>();
        createListener(7801, false, factory);
//        createListener(7896, false, factory);
//        createListener(7897, false, factory);
        g_IoContext.run();
    }
    
private:
    
};














