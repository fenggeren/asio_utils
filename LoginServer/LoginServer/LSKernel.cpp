//
//  LSKernel.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "LSKernel.hpp"
#include "LSSessionManager.hpp"
#include "GateSession.hpp"
#include "L2CSession.hpp"
#include <CPG/CPGHeader.h>
#include <Net/Conv.hpp>
#include <CPG/Util/ConvFunctional.hpp>
#include <CPG/Util/ServerConfigManager.hpp>

using namespace fasio::logging;


void LSKernel::start()
{
    auto& manager = ServerConfigManager::instance();
    manager.setType(ServerType_LoginServer);
    ServerNetConfig config;
    if (manager.configForType(ServerType_LoginServer,  config))
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

void LSKernel::runOneService(const ServerNetConfig::ServerInfo& config)
{
    auto& ioc = getIoContext();
    netInitializer(config, ioc, SessionManager);
    ioc.run();
}

std::shared_ptr<TCPSessionFactory>
LSKernel::sessionFactory(int type, asio::io_context& ioc)
{
    if (type == ServerType_GateServer)
    {
        return std::make_shared<GateSessionFactory>(ioc);
    }
    return nullptr;
}

void LSKernel::updateServiceConnect(std::shared_ptr<TCPSession> session,
                                    State state)
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
LSKernel::connectService(unsigned short type,
                         unsigned short port,
                         int sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, getIoContext(),  ip, port);
}

