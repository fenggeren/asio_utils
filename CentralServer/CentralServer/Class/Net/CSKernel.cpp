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
#include "CSMatchManager.hpp"
#include <Net/Conv.hpp>
#include "CSMatchLoadedEvaluation.hpp"
#include <CPG/Util/ServerConfigManager.hpp>

using namespace fasio::logging;

void CSKernel::start(const ServerNetConfig::ServerInfo& config)
{
 
    auto& curIoCtx = getIoContext();
    for(auto& listen: config.listenInfos)
    {
        SessionManager.createListener(listen.port, false,
                                      sessionFactory((ServerType)listen.type));
    }
    
    for(auto& connect : config.connectInfos)
    {
        SessionManager.createConnector(connect.type, curIoCtx,
                                       connect.ip,
                                       connect.port);
    }
    
    //
//    auto gateFactory = ;
//    SessionManager.createListener(7801, false, gateFactory);
//    auto matchFactory = ;
//    SessionManager.createListener(7802, false, matchFactory);
//    auto loginFactory = ;
//    SessionManager.createListener(7803, false, loginFactory);
//    auto balanceFactory = ;
//    SessionManager.createListener(7804, false, balanceFactory);
    
    CSMatchManager::instance().setChangedMatchMapCB(std::bind(&CSKernel::distributeMatch, this, std::placeholders::_1));
    
    curIoCtx.run();
}

std::shared_ptr<TCPSessionFactory>
CSKernel::sessionFactory(ServerType type)
{
    auto& curIoCtx = getIoContext();
    if (type == ServerType_GateServer) {
        return std::make_shared<GateSessionFactory>(curIoCtx);
    }
    else if (type == ServerType_LoginServer)
    {
        return std::make_shared<LoginSessionFactory>(curIoCtx);
    }
    else if (type == ServerType_MatchServer)
    {
        return std::make_shared<MatchSessionFactory>(curIoCtx);
    }
    else if (type == ServerType_BalanceServer)
    {
        return std::make_shared<BalanceSessionFactory>(curIoCtx);
    }
    return nullptr;
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
    auto iter = servers_.find(sid);
    if (iter == servers_.end())
    {
        LOG_MINFO << " not found service  sid: " << sid;
    }
    else
    {
        if (iter->second->type == ServerType_MatchServer)
        {
            CSMatchManager::instance().removeMatchService(iter->second);
        }
        servers_.erase(iter);
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
        if (rq.sid() == INVALID_LID ||
            rq.sid() >= servers_.size() ||
            servers_[rq.sid()] == nullptr)
        {
            info = std::make_shared<ServerInfo>();
            info->sid = session->uuid();
            session->setLogicID(info->sid);
            servers_[info->sid] = info;
            
            for(auto& lsn : rq.listeners())
            {
                info->listeners.push_back(
                {static_cast<unsigned char>(lsn.type()),
                    static_cast<unsigned short>(lsn.port()),
                    lsn.ip()});
            }
            for(auto t : rq.connecttypes())
            {
                info->connectTypes.push_back(t);
            }
        }
        else
        {
            info = servers_[rq.sid()];
        }
        uint32 cur = (uint32)time(NULL);
        info->type = session->type();
        info->connectTimes++;
        info->lastHeartTimes = cur;
        
 
        serverRegistRS(session, info);
        
        if (session->type() == ServerType_MatchServer)
        {
            CSMatchManager::instance().updateMatchService(info);
        }
        
        if (session->type() == ServerType_GateServer)
        {
            sendAllDistributeMatchInfos(session, ServerType_GateServer);
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


// 将消息直接返回给所有连接的 GS
void CSKernel::serverRegistRS(TCPSessionPtr session, std::shared_ptr<ServerInfo> info)
{
    // 获取需要连接的服务信息
    // 注册返回
    CPGToCentral::ServerRegisterRS rs;
    rs.set_result(0);
    rs.set_sid(info->sid);

    for(auto type : info->connectTypes)
    {
        for(auto& pair : servers_)
        {
            for (auto& listen : pair.second->listeners)
            {
                if (type == listen.type)
                {
                    auto conn = rs.add_connservers();
                    conn->set_port(listen.port);
                    conn->set_sid(pair.first);
                    conn->set_ip(listen.ip);
                    conn->set_type(type);
                }
            }
        }
    }
    SessionManager.sendMsgToSession(session, rs, kServerRegistRS);
    
    
    if (info->listeners.size() > 0)
    {
        for(auto& lis : info->listeners)
        {
            // 发送此服务信息给所有监听的类型服务信息
            CPGToCentral::NewConnServiceNotify notify;
            auto conn = notify.add_connservers();
            conn->set_port(lis.port);
            conn->set_sid(info->sid);
            conn->set_ip(lis.ip);
            conn->set_type(info->type);
            auto packet = NetPacket::createPacket(notify.SerializeAsString(), kServerNewServicesNotify);
            // 发送给所有的 所监听的服务
            SessionManager.sendMsgToSession(INVALID_UUID, packet, lis.type);
        }
    }

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
            for(auto& lis : smalleastLoadedGS->listeners)
            {
                if (lis.type == ServerType_Client)
                {
                    rs.set_port(lis.port);
                    rs.set_ip(lis.ip);
                    break;
                }
            }
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

void CSKernel::distributeMatch(const std::map<unsigned int, std::list<int>>& updateMap)
{
    LOG_MINFO << "";
 
    for(auto& pair : updateMap)
    {
        CPGToCentral::ServiceMatchDistibuteNotify notify;
        notify.set_sid(pair.first);
        for(auto mid : pair.second)
        {
            notify.add_mid(mid);
        }
        
        LOG_MINFO << " sendTo MS"
        << " sid: " << pair.first
        << " mids: " << jointContainer(pair.second);
        
        // 2.将sid对应的比赛列表 发送给该sid -- MS
        PacketHeader header{kServerMatchDistributeNotify,
            notify.ByteSize(), 0};
        SessionManager.sendMsg(getService(pair.first)->sid,
                               notify.SerializeAsString().data(),
                               header);
    }
    

    sendAllDistributeMatchInfos(nullptr, ServerType_GateServer);
}

void CSKernel::sendAllDistributeMatchInfos(TCPSessionPtr session, int stype)
{
    // 1. 将所有的分配信息发送个给所有的GS
    CPGToCentral::ServerAllMatchDistributeNotify notify;
    auto map = CSMatchManager::instance().getAllMatchServices();
    for(auto& pair : map)
    {
        CPGToCentral::ServiceMatchDistibuteNotify notify;
        notify.set_sid(pair.first.sid);
        for(auto mid : pair.second)
        {
            notify.add_mid(mid);
        }
        
        LOG_MINFO << "sendToGS "
        << " sid: " << pair.first.sid
        << " mids: " << jointContainer(pair.second);
    }
    
    // 如果 GS还再连接着MS A，
    // 但是MS A已经从CS断开
    // 是否需要清除掉 比赛分配信息？
    PacketHeader header{kServerMatchDistributeNotify, notify.ByteSize(), 0};
    SessionManager.sendMsg(session,
                           notify.SerializeAsString().data(),
                           header, stype);
}

void CSKernel::checkMatchDistributeRQ(TCPSessionPtr session, const void* data,
                            const PacketHeader& header)
{
    CPGToCentral::CheckMatchDistributeRQ rq;
    if (parseProtoMsg(data, header.size, rq))
    {
        std::list<int> mids;
        for (int i = 0; i <rq.mids_size(); i++)
        {
            mids.push_back(rq.mids(i));
        }
        
        int loaded = CSMatchLoadedEvaluation::evaluate(session->logicID(),
                                          mids);
        MatchDisService service{session->logicID(), loaded};
        auto res = CSMatchManager::instance().checkServiceDistMap(service, mids);
        
        CPGToCentral::CheckMatchDistributeRS rs;
        rs.set_sid(session->logicID());
        rs.set_type(session->type());
        rs.set_result(res);
        if (res == CSMatchManager::ErrorMatchServerMore
            || res == CSMatchManager::ErrorMatchServerMore)
        {
            for(auto mid : mids)
            {
                rs.add_mids(mid);
            }
        }
        
        PacketHeader header{kCheckMatchDistributeRS, rs.ByteSize(),
            static_cast<int32>(session->logicID())};
        SessionManager.sendMsg(session, rs.SerializeAsString().data(), header);
    }
}











