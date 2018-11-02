//
//  CSKernel.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//
#pragma once
#include <list>
#include <unordered_map>
#include <vector>
#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include <google/protobuf/message.h>
#include <CPG/CPGHeader.h>
#include <CPG/CPGServerDefine.h>
#include "GateSession.hpp"
#include "MatchSession.hpp"
#include "LoginSession.hpp"
#include "CSSessionManager.hpp"

using namespace fasio;

static asio::io_context g_IoContext;


// 网络中心
class CSKernel
{
public:
    
    void start()
    {
        //
        auto gateFactory = std::make_shared<GateSessionFactory>(g_IoContext);
        CSSessionManager::instance().createListener(7801, false, gateFactory);
        auto matchFactory = std::make_shared<MatchSessionFactory>(g_IoContext);
        CSSessionManager::instance().createListener(7802, false, matchFactory);
        auto loginFactory = std::make_shared<LoginSessionFactory>(g_IoContext);
        CSSessionManager::instance().createListener(7803, false, loginFactory);
        g_IoContext.run();
    }
    
public:
    
    std::shared_ptr<ServerInfo> getService(uint32 sid);
    void removeService(uint32 sid);
    
private:
    
    std::unordered_map<uint32, std::shared_ptr<ServerInfo>> servers_;
};



