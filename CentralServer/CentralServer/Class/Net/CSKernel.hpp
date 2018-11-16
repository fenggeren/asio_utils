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
#include <Net/ServiceKernel.hpp>
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
    // 注册一个新服务。
    // ①需要将其 监听的端口信息发送给所有 对应类型的服务.
    // ②还需要将其 需要连接的服务信息，返回给他。
    // !需要祛除掉CS服务。
    void serverRegistRQ(TCPSessionPtr session,
                         const void* data, int len);
     
    
    // 客户端连接BS 转发到this， 获取最合适的GS信息
    void requestBestGateServer(TCPSessionPtr session, const void* data,
                               const PacketHeader& header);
    
    
    
public:
    // MatchServer
    void checkMatchDistributeRQ(TCPSessionPtr session, const void* data,
                                const PacketHeader& header);
    
    
private:
     
    // @stype 分发给指定的服务
    void serverRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
    
    // 发送所有的比赛分配信息
    void sendAllDistributeMatchInfos(TCPSessionPtr session, int type);
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



