//
//  CSKernel.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSKernel.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGClient.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <Net/Util/NetPacket.hpp>
#include "CPGServerDefine.h"
#include "CSSessionManager.hpp"
#include "BalanceSession.hpp"
#include "GateSession.hpp"
#include "MatchSession.hpp"
#include "LoginSession.hpp"

using namespace fasio::logging;

void CSKernel::start()
{
    //
    auto gateFactory = std::make_shared<GateSessionFactory>(g_IoContext);
    SessionManager.createListener(7801, false, gateFactory);
    auto matchFactory = std::make_shared<MatchSessionFactory>(g_IoContext);
    SessionManager.createListener(7802, false, matchFactory);
    auto loginFactory = std::make_shared<LoginSessionFactory>(g_IoContext);
    SessionManager.createListener(7803, false, loginFactory);
    auto balanceFactory = std::make_shared<BalanceSessionFactory>(g_IoContext);
    SessionManager.createListener(7804, false, balanceFactory);
    
    g_IoContext.run();
}

std::shared_ptr<ServerInfo> CSKernel::getService(uint32 sid)
{
    std::shared_ptr<ServerInfo> info = nullptr;
    auto iter = servers_.find(sid);
    if (iter == servers_.end())
    {
        servers_[sid] = std::make_shared<ServerInfo>();
    }
    return servers_[sid];
}

void CSKernel::removeService(uint32 sid)
{
    if (servers_.erase(sid) == 0)
    {
        LOG_MINFO << " not found service  sid: " << sid;
    }
}


int32 CSKernel::serverID = 0;
void CSKernel::serverRegistRQ(TCPSessionPtr session,
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
            session->setLogicID(info->sid);
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
            distServices(info, ServerType_GateServer);
        }
        else
        {
            serverRegistRS(session, info);
        }
    }
    else
    {
        CPGToCentral::ServerRegisterRS rs;
        rs.set_result(-1);
        SessionManager.sendMsgToSession(session, rs, kServerRegistRS);
        LOG_ERROR << " cant parse proto msg len: " << len ;
    }
    
}

// 将所有 LoginServer & MatchServer信息返回给 GateServer
void CSKernel::gateServerRegistRS(TCPSessionPtr session,
                                          std::shared_ptr<ServerInfo> info)
{
    // 响应消息
    CPGToCentral::ServerRegisterRS rs;
    rs.set_result(0);
    rs.set_sid(info->sid);
    for (auto& server : servers_)
    {
        auto& serinfo = server.second;
        if (serinfo->type == ServerType_LoginServer ||
            serinfo->type == ServerType_MatchServer)
        {
            auto conn = rs.add_connservers();
            conn->set_port(serinfo->port);
            conn->set_sid(serinfo->sid);
            conn->set_ip(serinfo->ip);
            conn->set_type(serinfo->type);
        }
    }
    SessionManager.sendMsgToSession(session, rs, kServerRegistRS);
}

// 将消息直接返回给所有连接的 GS
void CSKernel::serverRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info)
{
    CPGToCentral::ServerRegisterRS rs;
    rs.set_result(0);
    rs.set_sid(info->sid);
    
    SessionManager.sendMsgToSession(session, rs, kServerRegistRS);
}

void CSKernel::distServices(std::shared_ptr<ServerInfo> info, uint8 stype)
{
    CPGToCentral::NewConnServiceNotify rs;
    auto conn = rs.add_connservers();
    conn->set_port(info->port);
    conn->set_sid(info->sid);
    conn->set_ip(info->ip);
    conn->set_type(info->type);
    
    auto packet = NetPacket::createPacket(rs.SerializeAsString(), kServerNewServicesNotify);
    SessionManager.sendMsgToSession(INVALID_UUID, packet, stype);
}

void CSKernel::serverLoginRQ(TCPSessionPtr session,
                                     const void* data, int len)
{
    CPGToCentral::ServerLoginRS rs;
    
    CPGToCentral::ServerLoginRQ rq;
    if (fasio::parseProtoMsg(data, len, rq))
    {
        std::shared_ptr<ServerInfo> smalleastLoadedGS = nullptr;
        int loaded = INT_MAX;
        for (auto& pair : servers_)
        {
            if (pair.second->type == ServerType_GateServer)
            {
                if (loaded > pair.second->loaded)
                {
                    loaded = pair.second->loaded;
                    smalleastLoadedGS = pair.second;
                }
            }
        }
        
        if (smalleastLoadedGS)
        {
            auto gsinfo = rs.mutable_gsinfo();
            gsinfo->set_port(smalleastLoadedGS->port);
            gsinfo->set_sid(smalleastLoadedGS->sid);
            gsinfo->set_ip(smalleastLoadedGS->ip);
            gsinfo->set_exportip(smalleastLoadedGS->exportIp);
        }
        else
        {
            rs.set_result(-1);
            LOG_ERROR << "has no valid gate server  all size: "
                    << servers_.size();
        }
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << "invalid data len: " << len;
    }
    SessionManager.sendMsgToSession(session, rs, kLoginRS);
}


void CSKernel::requestBestGateServer(TCPSessionPtr session,
                                     const void* data,
                                     const PacketHeader& header)
{
    CPGClient::ConnectRS rs;
    
    CPGClient::ConnectRQ rq;
    if (parseProtoMsg(data, header.size, rq))
    {
        rs.set_result(0); 
        std::shared_ptr<ServerInfo> smalleastLoadedGS = nullptr;
        int loaded = INT_MAX;
        for (auto& pair : servers_)
        {
            if (pair.second->type == ServerType_GateServer)
            {
                if (loaded > pair.second->loaded)
                {
                    loaded = pair.second->loaded;
                    smalleastLoadedGS = pair.second;
                }
            }
        }
        
        if (smalleastLoadedGS)
        {
            rs.set_ip(smalleastLoadedGS->exportIp);
            rs.set_port(smalleastLoadedGS->port);
        }
        else
        {
            rs.set_result(-1);
            LOG_ERROR << "has no valid gate server  all size: "
            << servers_.size();
        }
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << "invalid data len: " << header.size;
    }
    
    auto body = rs.SerializeAsString();
    SessionManager.sendMsg(session, body.data(),
                           {kConnectRS, rs.ByteSize(),
                            header.extraID});
}










