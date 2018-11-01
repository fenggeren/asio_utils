//
//  MSSessionManager.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include "GateSession.hpp"
#include "M2CSession.hpp"

using namespace fasio;

static asio::io_context g_IoContext;

  

class MSSessionManager : public TCPSessionManager
{
public:
    
    void start()
    {
        auto factory = std::make_shared<GateSessionFactory>(g_IoContext);
        createListener(7851, false, factory);
        createConnector(ServerType_Match_Central, g_IoContext, "127.0.0.1", 7802);
        g_IoContext.run();
    }
    
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override
    {
        if (type == ServerType_Match_Central)
        {
            return std::make_shared<M2CSession>();
        }
        else
        {
            return TCPSessionManager::createConnectorSession(type);
        }
    }
    
    
private:
    
    std::vector<std::shared_ptr<std::thread>> threads_;
    
};


