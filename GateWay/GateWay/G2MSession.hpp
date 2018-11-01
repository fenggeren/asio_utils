//
//  G2MSession.hpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//
#pragma once
#include <Net/TCPSession.hpp>

using namespace fasio;

// GateServer -> MatchServer
class G2MSession : public ClientSession
{
public:
    G2MSession():
    ClientSession()
    {
        messageCallback_ = std::bind(&G2MSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        connectionCallback_ = std::bind(&G2MSession::defaultConnectionCallback, this, std::placeholders::_1);
    }
    
private:
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data)
    {
        std::string content(data->peek(), data->readableBytes());
        data->retrieveAll();
        session->send(content);
    }
    
    void defaultConnectionCallback(const TCPSessionPtr& session)
    {
        session->send("HELLO Match Server");
    }
    
private:
};


