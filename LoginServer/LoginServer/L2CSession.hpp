//
//  L2CSession.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <Net/TCPSession.hpp>
using namespace fasio;


class L2CSession : public ClientSession
{
public:
    L2CSession():
    ClientSession()
    {
        messageCallback_ = std::bind(&L2CSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
        connectionCallback_ = std::bind(&L2CSession::defaultConnectionCallback, this, std::placeholders::_1);
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
        session->send("HELLO Central Server");
    }
    
private:
};


