//
//  BSSessionManager.hpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp>
#include <CPG/CPGHeader.h>

using namespace fasio;


#define SessionManager BSSessionManager::instance()

class BSSessionManager : public TCPSessionManager
{
public:
    
    static BSSessionManager& instance()
    {
        static BSSessionManager manager;
        return manager;
    }
     
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override;
     
};

