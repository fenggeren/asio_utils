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

using namespace fasio;

static asio::io_context g_IoContext;


// 网络中心
class CSKernel
{
public:
    
    static CSKernel& instance()
    {
        static CSKernel kernel;
        return kernel;
    }
    
    
    void start();
    
public:
    
    // sessionID, server type,
    void serverRegistRQ(TCPSessionPtr session,
                         const void* data, int len);
    void serverLoginRQ(TCPSessionPtr session,
                       const void* data, int len);
    
public:
    void requestBestGateServer(TCPSessionPtr session, const void* data, int len);
    
private:
    
    void gateServerRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
    void serverRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
public:
    
    std::shared_ptr<ServerInfo> getService(uint32 sid);
    void removeService(uint32 sid);
    
private:
    
    std::unordered_map<uint32, std::shared_ptr<ServerInfo>> servers_;
     
private:
    static int32 serverID;
};



