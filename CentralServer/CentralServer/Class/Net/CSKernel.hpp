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
#include <Net/TCPSession.hpp>
#include <CPG/CPGHeader.h>
#include <CPG/CPGServerDefine.h>
#include <CPG/Util/ServerConfigManager.hpp>


using namespace fasio;

namespace fasio
{
    class TCPSessionFactory;
}

// 网络中心
class CSKernel
{
public:
    
    static CSKernel& instance()
    {
        static CSKernel kernel;
        return kernel;
    }
    
    
    void start(const ServerNetConfig::ServerInfo& config);
public:
    
    // sessionID, server type,
    void serverRegistRQ(TCPSessionPtr session,
                         const void* data, int len);
    void serverLoginRQ(TCPSessionPtr session,
                       const void* data, int len);
    
    void requestBestGateServer(TCPSessionPtr session, const void* data,
                               const PacketHeader& header);
    
    
    
public:
    // MatchServer
    void checkMatchDistributeRQ(TCPSessionPtr session, const void* data,
                                const PacketHeader& header);
    
    
private:
    
    void gateServerRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
    // @stype 分发给指定的服务
    void serverRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
    void distServices(std::shared_ptr<ServerInfo> info, uint8 stype);
    
    
private:
    
    // 重新分配比赛
    void distributeMatch(const std::map<unsigned int, std::list<int>>& updateMap);
    
    std::shared_ptr<TCPSessionFactory>
    sessionFactory(ServerType type);
public:
    
    std::shared_ptr<ServerInfo> getService(uint32 sid);
    void removeService(uint32 sid);
    
private:
    std::unordered_map<uint32, std::shared_ptr<ServerInfo>> servers_;
    
    static int32 serverID;
};



