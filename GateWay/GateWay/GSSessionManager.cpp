//
//  GSSessionManager.cpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "GSSessionManager.hpp"
#include <Net/FASIO.hpp>
#include <CPG/CPGServerDefine.h>
#include "G2CSession.hpp"
#include "G2MSession.hpp"
#include "G2LSession.hpp"

using namespace fasio::logging;




void GSSessionManager::initialize()
{
    // 客户端心跳包检测。
    //
    TimerManager::createTimer(std::bind(&GSSessionManager::clientCheckOvertime, this), getIoContext(), kClientHeartBeatDuration, kClientHeartBeatDuration, INT_MAX);
}


void GSSessionManager::clientCheckOvertime()
{
    std::list<TCPSessionPtr> overtimeSessions;
    
    time_t cur = time(NULL);
//    LOG_DEBUG << cur;
    sessionMap_.foreach([&](const std::shared_ptr<TCPSession>& session)
                        {
                            if (session->type() == ServerType_Client)
                            {
                                LOG_DEBUG << session->heartBeatTime()
                                << "  over  " << cur - session->heartBeatTime();
                                if (cur - session->heartBeatTime() > kClientHeartBeatOvertime)
                                {
                                    overtimeSessions.push_back(session);
                                }
                            }
                        });
    
    
    for(auto& session : overtimeSessions)
    {
        LOG_MINFO << " client overtime " << session->uuid();
        session->forceClose();
    }
}


std::shared_ptr<ClientSession>
GSSessionManager::createConnectorSession(uint8 type)
{
    if (type == ServerType_CentralServer)
    {
        return std::make_shared<G2CSession>();
    }
    else if (type == ServerType_MatchServer)
    {
        return std::make_shared<G2MSession>();
    }
    else if (type == ServerType_LoginServer)
    {
        return std::make_shared<G2LSession>();
    }
    else
    {
        return TCPSessionManager::createConnectorSession(type);
    }
    
}
