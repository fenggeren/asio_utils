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
#include <CPG/CPGHeader.h>

using namespace fasio::logging;


void MSKernel::start()
{
    auto factory = std::make_shared<GateSessionFactory>(g_IoContext);
    SessionManager.createListener(7851, false, factory);
    centralSession_ = SessionManager.createConnector(ServerType_CentralServer, g_IoContext, "127.0.0.1", 7802);
    g_IoContext.run();
    
}

void MSKernel::removeConnectService(int uuid)
{
    connectServices_.erase(uuid);
    
    if (centralSession_ && uuid == centralSession_->uuid())
    {
        centralSession_ = nullptr;
    }
}
std::shared_ptr<TCPSession>
MSKernel::connectService(unsigned short type,
                         unsigned short port,
                         unsigned short sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, g_IoContext,  ip, port);
}

