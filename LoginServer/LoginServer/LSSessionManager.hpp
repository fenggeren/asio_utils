//
//  LSSessionManager.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <Net/TCPSession.hpp>
#include <Net/TCPSessionManager.hpp>


#define SessionManager LSSessionManager::instance()

using namespace fasio;



class LSSessionManager : public TCPSessionManager
{
public:
    
    static LSSessionManager& instance()
    {
        static LSSessionManager manager;
        return manager;
    }
    
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override;
     
private:
    
};

