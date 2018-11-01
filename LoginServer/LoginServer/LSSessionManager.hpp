//
//  LSSessionManager.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include <CPG/CPGServerDefine.h>
#include "GateSession.hpp"
#include "L2CSession.hpp"

using namespace fasio;

static asio::io_context g_IoContext;



class LSSessionManager : public TCPSessionManager
{
public:
    
    void start()
    {
        auto factory = std::make_shared<GateSessionFactory>(g_IoContext);
        createListener(7831, false, factory);
        createConnector(ServerType_Login_Central, g_IoContext, "127.0.0.1", 7803);
        g_IoContext.run();
    }
    
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override
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
     
private:
    
};

