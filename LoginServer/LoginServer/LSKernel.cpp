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
   SessionManager.createConnector(ServerType_Login_Central, g_IoContext, "127.0.0.1", 7803);
    g_IoContext.run();
}

void LSKernel::addNewConnect(int type, int port, int serverid,
                             const std::string& ip)
{
    if(SessionManager.getClientSession(serverid))
    {
        // 已经连接到 该服务. 添加连接状态？！
        LOG_MINFO << " has connect service: type" << type
        << " port: " << port
        << " ip: " << ip
        << " serverid: " << serverid;
    }
    else
    {
        auto clientSession =  SessionManager.createConnector(type, g_IoContext, ip, port);
        clientSession->setLogicID(serverid);
    }
}
