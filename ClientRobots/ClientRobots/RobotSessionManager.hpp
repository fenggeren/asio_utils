//
//  RobotSessionManager.hpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once


#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp>
#include <CPG/CPGHeader.h>


using namespace fasio;


#define SessionManager RobotSessionManager::instance()

class RobotSessionManager : public TCPSessionManager
{
public:
    
    static RobotSessionManager& instance()
    {
        static RobotSessionManager manager;
        return manager;
    }
    
public:
    
    
    
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override;
    
     
};
