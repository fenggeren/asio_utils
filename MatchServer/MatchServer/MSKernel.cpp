//
//  MSKernel.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "MSKernel.hpp"
#include "MSSessionManager.hpp"
#include "GateSession.hpp"
#include "M2CSession.hpp"
#include "MSMatchManager.hpp"
#include <CPG/CPGHeader.h>
#include <Net/FASIO.hpp>
#include <CPG/CPGServer.pb.h>
#include <CPG/CPGClientServer.pb.h>

using namespace fasio::logging;

static asio::io_context g_IoContext;

void MSKernel::start()
{
    auto& manager = ServerConfigManager::instance();
    manager.setType(ServerType_MatchServer);
    ServerNetConfig config;
    if (manager.configForType(ServerType_MatchServer,  config))
    {
        for(auto& info : config.infos)
        {
            runOneService(info);
        }
        getIoContext().run();
    }
    else
    {
        assert(0);
    }
}

void MSKernel::runOneService(const ServerNetConfig::ServerInfo& config)
{
    netInitializer(config, getIoContext(), SessionManager);
    
}

std::shared_ptr<TCPSessionFactory>
MSKernel::sessionFactory(int type, asio::io_context& ioc)
{
    if (type == ServerType_GateServer)
    {
        return std::make_shared<GateSessionFactory>(ioc);
    }
    return nullptr;
}

void MSKernel::updateServiceConnect(std::shared_ptr<TCPSession> session, State state)
{
    if (state == kClose)
    {
        if (centralSession_ == session)
        {
            centralSession_ = nullptr;
        }
    }
    else
    {
        if (session->type() == ServerType_CentralServer)
        {
            centralSession_ = session;
        }
    }
}

 

void MSKernel::transToCS(const void* data, const PacketHeader& header)
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
void MSKernel::sendMsg(const TCPSessionPtr& session,
             google::protobuf::Message& msg,
             int type, int extraID)
{
    PacketHeader header{kCheckMatchDistributeRQ, msg.ByteSize(), extraID};
    SessionManager.sendMsg(session, msg.SerializeAsString().data(), header);
}

std::shared_ptr<TCPSession>
MSKernel::connectService(unsigned short type,
                         unsigned short port,
                         int sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, getIoContext(),  ip, port);
}


void MSKernel::checkMatchDistributeRS(const void* buffer, const PacketHeader& header)
{
    LOG_MINFO << "";
    CPGServer::CheckMatchDistributeRS rs;
    if (parseProtoMsg(buffer, header.size, rs))
    {
        LOG_MINFO << " result: " << rs.result();
        if (rs.result() == 1 ||
            rs.result() == 2)
        {
            std::list<int> mids;
            for(int i = 0; i < rs.mids_size(); i++)
            {
                mids.push_back(rs.mids(i));
            }
            MSMatchManager::instance().updateMatches(mids);
            LOG_MINFO << " valid mids: " << jointContainer(mids);
        }
    }
}

void MSKernel::distibuteMatchesNotify(const void* buffer, const PacketHeader& header)
{
    LOG_MINFO << "";
    CPGServer::ServiceMatchDistibuteNotify notify;
    if (parseProtoMsg(buffer, header.size, notify))
    {
        std::list<int> mids;
        for(int i = 0; i < notify.mid_size(); i++)
        {
            int mid = notify.mid(i);
            mids.push_back(mid);
        }
        MSMatchManager::instance().updateMatches(mids);
        
        LOG_MINFO << "distribute matches: " << jointContainer(mids);
    }
}


// 报名比赛
void MSKernel::joinMatchRQ(const TCPSessionPtr& session,
                           const void* buffer,
                           const PacketHeader& header)
{
    LOG_MINFO << "";
    
     CPGClientServer::JoinMatchRS rs;
    CPGClientServer::JoinMatchRQ rq;
    if (parseProtoMsg(buffer, header.size, rq))
    {
        rs.set_result(0);
        // 报名比赛
        int remain = MSMatchManager::instance().joinMatch(rq.uid(), rq.mid(), rq.mtype());
        rs.set_remainplayer(remain);
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << header.type << " size: " << header.size;
    }
    rs.set_uid(rq.uid());
    rs.set_mid(rq.mid());
    rs.set_mtype(rq.mtype());
    
    PacketHeader rsHeader{kJoinMatchRS, rs.ByteSize(), header.extraID};
    SessionManager.sendMsg(session, rs.SerializeAsString().data(),
                           rsHeader);
}

// 取消报名比赛
void MSKernel::unjoinMatchRQ(const TCPSessionPtr& session,
                             const void* buffer,
                             const PacketHeader& header)
{
    LOG_MINFO << "";
    
    CPGClientServer::UnjoinMatchRS rs;
    
    CPGClientServer::UnjoinMatchRQ rq;
    if (parseProtoMsg(buffer, header.size, rq))
    {
        rs.set_result(0);
        // 报名比赛
        int remain = MSMatchManager::instance().unjoinMatch(rq.uid(), rq.mid(), rq.mtype());
        rs.set_remainplayer(remain);
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << header.type << " size: " << header.size;
    }
    rs.set_uid(rq.uid());
    rs.set_mid(rq.mid());
    rs.set_mtype(rq.mtype());
    
    PacketHeader rsHeader{kUnjoinMatchRS, rs.ByteSize(), header.extraID};
    SessionManager.sendMsg(session, rs.SerializeAsString().data(),
                           rsHeader);
}





