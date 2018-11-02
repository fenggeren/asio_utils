//
//  CSSessionManager.cpp
//  CentralServer
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSSessionManager.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <Net/Util/NetPacket.hpp>
#include "CPGServerDefine.h"

using namespace fasio::logging;

int32 CSSessionManager::serverID = 0;
void CSSessionManager::serviceRegistRQ(TCPSessionPtr session,
                                       const void* data, int len)
{
    CPGToCentral::ServerRegisterRQ rq;
    if (fasio::parseProtoMsg(data, len, rq))
    {
        std::shared_ptr<ServerInfo> info = nullptr;
        if (rq.sid() == 0 ||
            rq.sid() >= servers_.size() ||
            servers_[rq.sid()] == nullptr)
        {
            info = std::make_shared<ServerInfo>();
            info->sid = ++serverID;
            servers_[info->sid] = info;
        }
        else
        {
            info = servers_[rq.sid()];
        }
        
        uint32 cur = (uint32)time(NULL);
        info->type = session->type();
        info->port = rq.port();
        info->ip = rq.ip();
        info->exportIp = rq.exportip();
        info->connectTimes++;
        info->lastHeartTimes = cur;
        
        
        if (session->type() == ServerType_GateServer)
        {
            gateServerRegistRS(session ,info);
        }
        else if (session->type() == ServerType_LoginServer ||
                 session->type() == ServerType_MatchServer)
        {
            serverRegistRS(session ,info);
        }
        else
        {
            LOG_ERROR << "invalid type : " << session->type();
        }
    }
    else
    {
        CPGToCentral::ServerRegisterRS rs;
        rs.set_result(-1);
        sendMsgToSession(session, rs, kServerRegistRS);
        LOG_ERROR << " cant parse proto msg len: " << len ;
    }
    
}

// 将所有 LoginServer & MatchServer信息返回给 GateServer
void CSSessionManager::gateServerRegistRS(TCPSessionPtr session,
                                          std::shared_ptr<ServerInfo> info)
{
    // 响应消息
    CPGToCentral::ServerRegisterRS rs;
    rs.set_result(0);
    rs.set_sid(info->sid);
    for (auto& server : servers_)
    {
        if (server->type != ServerType_GateServer)
        {
            auto conn = rs.add_connservers();
            conn->set_port(server->port);
            conn->set_sid(server->sid);
            conn->set_ip(server->ip);
            conn->set_type(server->type);
        }
    }
    sendMsgToSession(session, rs, kServerRegistRS);
}

// 将消息直接返回给所有连接的 GS
void CSSessionManager::serverRegistRS(TCPSessionPtr session,
                                      std::shared_ptr<ServerInfo> info)
{
    CPGToCentral::ServerRegisterRS rs;
    rs.set_result(0);
    rs.set_sid(info->sid);
    auto conn = rs.add_connservers();
    conn->set_port(info->port);
    conn->set_sid(info->sid);
    conn->set_ip(info->ip);
    conn->set_type(info->type);

    auto packet = NetPacket::createPacket(rs.SerializeAsString(), kServerRegistRS);
    
    sendMsgToSession(0, packet, ServerType_GateServer);
}

void CSSessionManager::serverLoginRQ(TCPSessionPtr session,
                                    const void* data, int len)
{
    CPGToCentral::ServerLoginRS rs;
    
    CPGToCentral::ServerLoginRQ rq;
    if (fasio::parseProtoMsg(data, len, rq))
    {
        for (auto& pair : sessionMap_)
        {
            int loaded = INT_MAX;
            if (pair.second->type() == ServerType_GateServer)
            {
                
            }
        }
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << "invalid data len: " << len;
    }
    sendMsgToSession(session, rs, kServerLoginRS);
}










