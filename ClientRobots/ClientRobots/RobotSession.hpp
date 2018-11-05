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

// GateServer -> MatchServer
class RobotSession : public ClientSession
{
public:
    RobotSession():
    ClientSession()
    {
        firstConnect_ = true;
        messageCallback_ = std::bind(&RobotSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        connectionCallback_ = std::bind(&RobotSession::defaultConnectionCallback, this, std::placeholders::_1);
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

    void connectRS(const void* buffer, int len);

private:
    bool firstConnect_;
};





