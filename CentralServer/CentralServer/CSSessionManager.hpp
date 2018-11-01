//
//  CSSessionManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include "GateSession.hpp"
#include "MatchSession.hpp"
#include "LoginSession.hpp"

using namespace fasio;

static asio::io_context g_IoContext;




class CSSessionManager : public TCPSessionManager
{
public:
    
    void start()
    {
        //
        auto gateFactory = std::make_shared<GateSessionFactory>(g_IoContext);
        createListener(7801, false, gateFactory);
        auto matchFactory = std::make_shared<MatchSessionFactory>(g_IoContext);
        createListener(7802, false, matchFactory);
        auto loginFactory = std::make_shared<LoginSessionFactory>(g_IoContext);
        createListener(7803, false, loginFactory);
        g_IoContext.run();
    }
    
private:
    
};














