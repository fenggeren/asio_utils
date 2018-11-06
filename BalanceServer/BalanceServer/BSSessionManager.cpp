//
//  BSSessionManager.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "BSSessionManager.hpp"
#include <Net/logging/Logging.hpp>
#include <Net/Util/ParseProto.hpp>
#include <CPG/CPGServerDefine.h>
#include "B2CSession.hpp"
#include "ClientSession.hpp"

using namespace fasio::logging;


std::shared_ptr<ClientSession>
BSSessionManager::createConnectorSession(uint8 type)
{
    if (type == ServerType_CentralServer)
    {
        return std::make_shared<B2CSession>();
    }
    else
    {
        return TCPSessionManager::createConnectorSession(type);
    }
    
}
