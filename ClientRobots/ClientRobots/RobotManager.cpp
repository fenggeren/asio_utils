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
port_(7835)
{

}

void RobotManager::start()
{
    robots_.resize(robotNums_);
    for(int i = 0; i < robotNums_; ++i)
    {
        auto robot = std::make_shared<Robot>(ip_, port_);
        auto session = SessionManager.createConnector(ServerType_BalanceServer, io_context_, robot->ip(), robot->port());
        session->setLogicID(i);
        robot->setLogicID(session->logicID());
        robots_[i] = robot;
        if (i % 100 == 0)
        {
            usleep(10000);
        }
    }
    io_context_.run();
}

void RobotManager::postConnect(std::shared_ptr<Robot> robot)
{
    auto session = SessionManager.createConnector(ServerType_GateServer, io_context_, robot->ip(), robot->port());
    session->setLogicID(robot->logicID());
}







