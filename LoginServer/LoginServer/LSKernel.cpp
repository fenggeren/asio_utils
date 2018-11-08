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

using namespace fasio::logging;


void LSKernel::start()
{
    auto factory = std::make_shared<GateSessionFactory>(g_IoContext);
    SessionManager.createListener(7831, false, factory);
   centralSession_ = SessionManager.createConnector(ServerType_CentralServer, g_IoContext, "127.0.0.1", 7803);
    g_IoContext.run();
}


void LSKernel::removeConnectService(int uuid)
{
    connectServices_.erase(uuid);
    
    if (uuid == centralSession_->uuid())
    {
        centralSession_ = nullptr;
    }
}

std::shared_ptr<TCPSession>
LSKernel::connectService(unsigned short type,
                         unsigned short port,
                         unsigned short sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, g_IoContext,  ip, port);
}

