//
//  B2CSession.hpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <Net/TCPSession.hpp>

using namespace fasio;

// GateServer -> CentralServer
class B2CSession : public ClientSession
{
public:
    B2CSession():
    ClientSession()
    {
        messageCallback_ = std::bind(&B2CSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
//        connectionCallback_ = std::bind(&B2CSession::defaultConnectionCallback, this, std::placeholders::_1);
    }
    
private:
    
    virtual void onClose() override;
    virtual void sendInitData() override;
    
private:
    
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data);
    
    void defaultConnectionCallback(const TCPSessionPtr& session)
    {
    }
private:
    
    void serverRegistRS(const void* data, int len);
    void connectRS(const void* data, int len);
private:
};


