//
//  BSKernel.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "BSKernel.hpp"
#include <CPG/CPGServer.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <Net/Util/NetPacket.hpp>
#include "CPGServerDefine.h"
#include "BSSessionManager.hpp"
#include "ClientSession.hpp"
#include <Net/Conv.hpp>
#include <CPG/Util/ConvFunctional.hpp>
#include <CPG/Util/ServerConfigManager.hpp>


using namespace fasio::logging;

void BSKernel::start()
{
    auto& manager = ServerConfigManager::instance();
    manager.setType(ServerType_BalanceServer);
    ServerNetConfig config;
    if (manager.configForType(ServerType_BalanceServer,  config))
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
}

void BSKernel::runOneService(const ServerNetConfig::ServerInfo& config)
{
    auto& ioc = getIoContext();
    netInitializer(config, ioc, SessionManager);
    ioc.run();
}

std::shared_ptr<TCPSessionFactory>
BSKernel::sessionFactory(int type, asio::io_context& ioc)
{
    if (type == ServerType_Client)
    {
        return std::make_shared<CBSessionFactory>(ioc);
    }
    return nullptr;
}


 

void BSKernel::updateServiceConnect(std::shared_ptr<TCPSession> session, State state)
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

std::shared_ptr<TCPSession>
BSKernel::connectService(unsigned short type,
                         unsigned short port,
                         int sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, getIoContext(),  ip, port);
}

void BSKernel::transToCS(const void* data, const PacketHeader& header)
{
    LOG_MINFO << " ";
    if (centralSession_)
    {
        SessionManager.sendMsg(centralSession_, data, header);
    }
    else
    {
        LOG_ERROR << " not connect central server"
        << " msgid: " << header.type;
    }
}

void BSKernel::transToClient(const void* data, const PacketHeader& header)
{
    LOG_MINFO << " ";
    SessionManager.sendMsg(header.extraID, data, header);
}
