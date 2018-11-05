//
//  RobotManager.cpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "RobotManager.hpp"
#include "RobotSessionManager.hpp"

RobotManager::RobotManager()
:ip_("127.0.0.1"), 
port_(9278)
{

}

void RobotManager::start()
{
    for(int i = 0; i < robotNums_; ++i)
    {
        auto robot = std::make_shared<Robot>(ip_, port_);
        auto session = SessionManager.createConnector(ServerType_Client, io_context_, 
                                robot->ip(), robot->port());
        robot->setSessionID(session->uuid());
        robotsMap_[session->uuid()] = robot;
    }
 
}

void RobotManager::postConnect(std::shared_ptr<Robot> robot)
{

}