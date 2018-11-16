//
//  GSKernel.cpp
//  GateWay
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "ClientSession.hpp"
#include "GSKernel.hpp"
#include <CPG/CPGToCentral.pb.h>
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
}


void GSKernel::runOneService(const ServerNetConfig::ServerInfo& config)
{
    auto& ioc = getIoContext();
    netInitializer(config, ioc, SessionManager);
    ioc.run();
}

void GSKernel::distibuteMatchesNotify(const void* buffer, const PacketHeader& header)
{
    
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
                           unsigned short sid,
                           const std::string& ip)
{
    auto session = SessionManager.createConnector(type, g_IoContext,  ip, port);
    if (type == ServerType_LoginServer)
    {
        if (loginSession_)
        {
            LOG_MINFO << " has connect login server:"
            << " port: " << port
            << " sid: " << sid
            << " ip: " << ip;
        }
        loginSession_ = session;
    }
    return session;
}

void GSKernel::removeConnectService(int uuid)
{
    connectServices_.erase(uuid);
    
    if (loginSession_ && uuid == loginSession_->uuid())
    {
        loginSession_ = nullptr;
    }
    else if (centralSession_ && uuid == centralSession_->uuid())
    {
        centralSession_ = nullptr;
    }
}


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
        
    }
    else
    {
        LOG_ERROR << " not connect central server"
        << " msgid: " << header.type;
    }
}






