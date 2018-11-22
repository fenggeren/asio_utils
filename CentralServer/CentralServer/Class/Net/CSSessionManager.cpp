//
//  CSSessionManager.cpp
//  CentralServer
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSSessionManager.hpp"
#include <CPG/CPGServer.pb.h>
#include <Net/FASIO.hpp>
#include "CPGServerDefine.h"

using namespace fasio::logging;

void CSSessionManager::initialize()
{
    // 客户端心跳包检测。
    //
    TimerManager::createTimer(std::bind(&CSSessionManager::clientCheckOvertime, this), getIoContext(), kServerHeartBeatDuration, kServerHeartBeatDuration, INT_MAX);
}


void CSSessionManager::clientCheckOvertime()
{
    std::list<TCPSessionPtr> overtimeSessions;
    time_t cur = time(NULL);
    sessionMap_.foreach([&](const std::shared_ptr<TCPSession>& session)
                        {
//                            LOG_DEBUG << session->heartBeatTime()
//                            << "  over  " << cur - session->heartBeatTime();
                            if (cur - session->heartBeatTime() > kServerHeartBeatOvertime)
                            {
                                overtimeSessions.push_back(session);
                            }
                        });
    
    
    for(auto& session : overtimeSessions)
    {
        LOG_ERROR << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        << "service overtime " << session->uuid();
//        session->forceClose();
    }
}








