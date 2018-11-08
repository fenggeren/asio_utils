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

using namespace fasio::logging;

void BSKernel::start()
{
    auto factory = std::make_shared<CBSessionFactory>(g_IoContext);
    SessionManager.createListener(7835, false, factory);
    centralSession_ = SessionManager.createConnector(ServerType_CentralServer, g_IoContext, "127.0.0.1", 7804);
    g_IoContext.run();
}
 
void BSKernel::removeConnectService(int uuid)
{
    connectServices_.erase(uuid);
    
    if (uuid == centralSession_->uuid())
    {
        centralSession_ = nullptr;
    }
}

std::shared_ptr<TCPSession>
BSKernel::connectService(unsigned short type,
                         unsigned short port,
                         unsigned short sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, g_IoContext,  ip, port);
}

void BSKernel::transToCS(google::protobuf::Message& msg, int msgID, int clientID)
{
    if (centralSession_)
    {
        SessionManager.transMsgToSession(centralSession_, msg, msgID, clientID);
    }
    else
    {
        LOG_ERROR << " not connect central server"
        << " msgid: " << msgID;
    }
}

