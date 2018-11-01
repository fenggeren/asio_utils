//
//  G2LSession.hpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once
#include <Net/TCPSession.hpp>

using namespace fasio;

// GateServer -> LoginServer
class G2LSession : public ClientSession
{
public:
    G2LSession():
    ClientSession()
    {
        messageCallback_ = std::bind(&G2LSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        connectionCallback_ = std::bind(&G2LSession::defaultConnectionCallback, this, std::placeholders::_1);
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
        session->send("HELLO Login Server");
    }
    
private:
};
