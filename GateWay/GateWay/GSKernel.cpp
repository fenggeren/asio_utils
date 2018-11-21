//
//  GSKernel.cpp
//  GateWay
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "ClientSession.hpp"
#include "GSKernel.hpp"
#include <CPG/CPGServer.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <Net/Util/NetPacket.hpp>
#include "CPGServerDefine.h"
#include "GSSessionManager.hpp"
#include <Net/Conv.hpp>
#include <CPG/Util/ConvFunctional.hpp>
#include <CPG/Util/ServerConfigManager.hpp>

using namespace fasio::logging;

void GSKernel::start()
{
    auto& manager = ServerConfigManager::instance();
    manager.setType(ServerType_GateServer);
    ServerNetConfig config;
    if (manager.configForType(ServerType_GateServer,  config))
    {
        for(auto& info : config.infos)
        {
            runOneService(info);
        }
    }
    else
    {
        assert(0);
    }
    
    SessionManager.initialize();
    getIoContext().run();
}


void GSKernel::runOneService(const ServerNetConfig::ServerInfo& config)
{
    netInitializer(config, getIoContext(), SessionManager);
}

void GSKernel::distibuteMatchesNotify(const void* buffer, const PacketHeader& header)
{
    LOG_DEBUG << "";
    CPGServer::ServerAllMatchDistributeNotify notify;
    if (fasio::parseProtoMsg(buffer, header.size, notify))
    {
        // sid : <mid>
        std::map<int, std::list<int>> matchDistr;
        for(auto& service : notify.services())
        {
            auto iter = matchDistr.find(service.sid());
            if (iter != matchDistr.end())
            {
                LOG_MINFO << "old match distr sid: " << service.sid()
                << " mids: " << jointContainer(iter->second);
                matchDistr.erase(iter);
            }
            matchDistr[service.sid()].clear();
            for(auto mid : service.mid())
            {
                matchDistr[service.sid()].push_back(mid);
            }
            
            LOG_MINFO << "new match distr sid: " << service.sid()
            << " mids: " << jointContainer(service.mid());
        }
        
        // 转化---
        //        SessionManager.updateMatchDistri(matchDistr);
        for(auto& pair : matchDistr)
        {
            auto session = connectServiceSession(pair.first);
            if (session)
            {
                for(auto mid : pair.second)
                {
                    matchesServices_[mid] = session;
                }
            }
            else
            {
                LOG_ERROR << " cant found connect service session: " << pair.first;
            }
        }
        matchesDistriMaps_ = matchDistr;
    }
}

std::shared_ptr<TCPSessionFactory>
GSKernel::sessionFactory(int type, asio::io_context& ioc)
{
    if (type == ServerType_Client)
    {
        return std::make_shared<CGSessionFactory>(ioc);
    }
    return nullptr;
}


std::shared_ptr<TCPSession>
GSKernel::connectService(unsigned short type,
                           unsigned short port,
                           int sid,
                           const std::string& ip)
{
    return SessionManager.createConnector(type, getIoContext(),  ip, port);
}

void GSKernel::removeConnectMatchService(std::shared_ptr<TCPSession> session)
{
        // 比赛服， 移除相关比赛分配信息
        if (session->type() == ServerType_MatchServer)
        {
            std::list<int> rmvMids;
            for (auto iter = matchesServices_.begin(); iter != matchesServices_.end();)
            {
                if (iter->second == session)
                {
                    rmvMids.push_back(iter->first);
                    iter = matchesServices_.erase(iter);
                }
                else
                {
                    iter++;
                }
            }
            
            LOG_MINFO << "removed match service session for matches matches: "
                << jointContainer(rmvMids);
        }
    
        if (loginSession_ == session)
        {
            loginSession_ = nullptr;
        }
        else if (centralSession_ == session)
        {
            centralSession_ = nullptr;
        }
}
inline TCPSessionPtr GSKernel::matchServiceSession(int mid)
{
    auto iter = matchesServices_.find(mid);
    if (iter != matchesServices_.end())
    {
        return iter->second;
    }
    return nullptr;
}


void GSKernel::updateServiceConnect(std::shared_ptr<TCPSession> session,
                                    State state)
{
    if (state == kClose)
    {
        removeConnectMatchService(session);
    }
    else
    {
        if (session->type() == ServerType_LoginServer)
        {
            if (loginSession_)
            {
                LOG_MINFO << " has connect login server:"
                << loginSession_->name();
            }
            loginSession_ = session;
        }
        else if (session->type() == ServerType_CentralServer)
        {
            if (centralSession_)
            {
                LOG_MINFO << " has connect central server:"
                << centralSession_->name();
            }
            centralSession_ = session;
        }
    }
}


//////
void GSKernel::transToLS(const void* data, const PacketHeader& header)
{
    if (loginSession_)
    {
        SessionManager.sendMsg(loginSession_, data, header);
    }
    else
    {
        LOG_ERROR << " not connect login server"
        << " msgid: " << header.type;
    }
}

void GSKernel::transToCS(const void* data, const PacketHeader& header)
{
    if (centralSession_)
    {
        SessionManager.sendMsg(centralSession_, data, header);
    }
    else
    {
        LOG_ERROR << " not connect login server"
        << " msgid: " << header.type;
    }
}

void GSKernel::transToMS(const void* data, const PacketHeader& header, int mid)
{
    if (matchesServices_.size() > 0)
    {
        // mid 对应session 是否存在--
        auto session = matchServiceSession(mid);
        if (session)
        {
            SessionManager.sendMsg(session, data, header);
        }
        else
        {
            LOG_ERROR << " cant found match service for mid: " << mid;
        }
    }
    else
    {
        LOG_ERROR << " not connect match server"
        << " msgid: " << header.type;
    }
}

void GSKernel::transToClient(const void* data, const PacketHeader& header)
{
    LOG_MINFO << " ";
    SessionManager.sendMsg(header.extraID, data, header);
}


void GSKernel::beatHeartRQ(const std::shared_ptr<TCPSession>& session,
                           const void* buffer, const PacketHeader& header)
{
    
}




