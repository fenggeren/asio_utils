//
//  BSKernel.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "BSKernel.hpp"
#include <CPG/CPGToCentral.pb.h>
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
    manager.setType(ServerType_MatchServer);
    ServerNetConfig config;
    if (manager.configForType(ServerType_MatchServer,  config))
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


void BSKernel::removeConnectService(int uuid)
{
    connectServices_.erase(uuid);
    
    if (centralSession_ && uuid == centralSession_->uuid())
    {
        centralSession_ = nullptr;
    }
}

std::shared_ptr<TCPSession>
BSKernel::connectService(unsigned short type,
                         unsigned short port,
                         short sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, g_IoContext,  ip, port);
}

void BSKernel::transToCS(const void* data, const PacketHeader& header)
{
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

