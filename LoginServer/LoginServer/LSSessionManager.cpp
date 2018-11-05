//
//  LSSessionManager.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "LSSessionManager.hpp"
#include <CPG/CPGServerDefine.h>
#include "L2CSession.hpp"

std::shared_ptr<ClientSession> LSSessionManager::createConnectorSession(uint8 type) 
{
    if (type == ServerType_Login_Central)
    {
        return std::make_shared<L2CSession>();
    }
    else
    {
        return TCPSessionManager::createConnectorSession(type);
    }
    }
