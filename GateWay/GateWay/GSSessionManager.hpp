//
//  GSSessionManager.hpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp> 
#include <CPG/CPGHeader.h>
#include <list>

using namespace fasio;


#define SessionManager GSSessionManager::instance()


class GSSessionManager : public TCPSessionManager
{
public:
    
    static GSSessionManager& instance()
    {
        static GSSessionManager manager;
        return manager;
    }
    
    void initialize();
    
    // 客户端心跳包管理--
private:
    
    void clientCheckOvertime();
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override;
    
private:
    
};



