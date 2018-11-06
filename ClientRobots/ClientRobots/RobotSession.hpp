//
//  RobotSession.hpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <Net/TCPSession.hpp>

using namespace fasio;


class C2BSession : public ClientSession
{
public:
    
    C2BSession():
    ClientSession()
    {
        messageCallback_ = std::bind(&C2BSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
//        connectionCallback_ = std::bind(&C2BSession::defaultConnectionCallback, this, std::placeholders::_1);
    }
    
private:
    
    virtual void onClose() override;
    virtual void sendInitData() override;
    
private:
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data);
    
    void defaultConnectionCallback(const TCPSessionPtr& session)
    {
        session->send("HELLO Central Server");
    }
private:
    
    void connectRS(const void* buffer, int len);
};


// GateServer -> MatchServer
class RobotSession : public ClientSession
{
public:
    RobotSession():
    ClientSession()
    {
        messageCallback_ = std::bind(&RobotSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
//        connectionCallback_ = std::bind(&RobotSession::defaultConnectionCallback, this, std::placeholders::_1);
    }
    
public:

    virtual void sendInitData() override;

    virtual void onClose() override;
private:
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session, DataBuffer*const data);
    
    void defaultConnectionCallback(const TCPSessionPtr& session)
    {
        session->send("HELLO Match Server");
    }
    
private:

    void loginRS(const void* data, int len);
    
private: 
};





