//
//  RobotSessionManager.cpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "RobotSessionManager.hpp"
#include <Net/logging/Logging.hpp>
#include <Net/Util/ParseProto.hpp>
#include <CPG/CPGServerDefine.h>
#include "RobotSession.hpp"

std::shared_ptr<ClientSession>
RobotSessionManager::createConnectorSession(uint8 type)
{
    if (type == ServerType_BalanceServer)
    {
        return std::make_shared<C2BSession>();
    }
    else if (type == ServerType_GateServer)
    {
        return std::make_shared<RobotSession>();
    }
    else
    {
        return TCPSessionManager::createConnectorSession(type);
    }
    
}
