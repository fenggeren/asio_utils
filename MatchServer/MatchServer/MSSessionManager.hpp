//
//  MSSessionManager.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
 
#include <Net/TCPSessionManager.hpp>




#define SessionManager MSSessionManager::instance()
using namespace fasio;

class MSSessionManager : public TCPSessionManager
{
public:
    
    static MSSessionManager& instance()
    {
        static MSSessionManager manager;
        return manager;
    }
    

    
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override;
    
    
private:
    
};


