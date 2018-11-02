//
//  CSSessionManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <list>
#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include <google/protobuf/message.h>
#include <CPG/CPGHeader.h>
#include <CPG/CPGServerDefine.h>
#include "GateSession.hpp"
#include "MatchSession.hpp"
#include "LoginSession.hpp"

using namespace fasio;

class CSSessionManager : public TCPSessionManager
{
public:
    
    static CSSessionManager& instance()
    {
        static CSSessionManager manager;
        return manager;
    }
    
     
public:
    // sessionID, server type,
    void serviceRegistRQ(TCPSessionPtr session,
                         const void* data, int len);
    void serverLoginRQ(TCPSessionPtr session,
                       const void* data, int len);
     
private:
    
    void gateServerRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
    void serverRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info);
private:
    std::vector<std::shared_ptr<ServerInfo>> servers_;
    
private:
    static int32 serverID;
};














