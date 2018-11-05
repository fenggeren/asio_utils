//
//  GSKernel.hpp
//  GateWay
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp>
#include <Net/DataBuffer.hpp>
#include <CPG/CPGHeader.h>
#include "ClientSession.hpp"
#include "G2CSession.hpp"
#include "G2MSession.hpp"
#include "G2LSession.hpp"
#include "GSSessionManager.hpp"

using namespace fasio;
static asio::io_context g_IoContext;

struct MatchServerInfo
{
    std::shared_ptr<G2MSession> matchSession_;
    std::shared_ptr<ServerInfo> info_;
};

class GSKernel
{
public:
    
    static GSKernel& instance()
    {
        static GSKernel kernel;
        return kernel;
    }
    
    void start()
    {
        auto factory = std::make_shared<CGSessionFactory>(g_IoContext);
        GSSessionManager::instance().createListener(7890, false, factory);
        
        GSSessionManager::instance().createConnector(ServerType_Gate_Central, g_IoContext, "127.0.0.1", 7801);
        GSSessionManager::instance().createConnector(ServerType_Gate_Match, g_IoContext, "127.0.0.1", 7851);
        GSSessionManager::instance().createConnector(ServerType_Gate_Login, g_IoContext, "127.0.0.1", 7831);
        g_IoContext.run();
    }
    
public:
    void addNewConnect(int type, int port, int serverid, const std::string& ip);
    
public:
    
    void serverRegistRS(TCPSessionPtr session,
                        const void* data, int len);
    void serverLoginRS(TCPSessionPtr session,
                       const void* data, int len);
    
private:
    // 比赛id <=> match info
    std::unordered_map<uint32, TCPSessionPtr> matchesServices_;
};





