//
//  MSSessionManager.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "MSSessionManager.hpp"
#include "M2CSession.hpp"
#include <CPG/CPGHeader.h>
#include <CPG/CPGServerDefine.h>

std::shared_ptr<ClientSession> MSSessionManager::createConnectorSession(uint8 type) 
{
    if (type == ServerType_CentralServer)
    {
        return std::make_shared<M2CSession>();
    }
    else
    {
        return TCPSessionManager::createConnectorSession(type);
    }
    }
