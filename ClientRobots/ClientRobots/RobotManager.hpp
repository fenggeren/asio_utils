//
//  RobotManager.hpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/CPGServerDefine.h>
#include <Net/base/FASIOType.h>
#include <Net/Header.h>
#include <unordered_map>
#include <vector>
#include "Robot.hpp"


#define gRobotManager RobotManager::instance()

class RobotManager
{
public:
    
    static RobotManager& instance()
    {
        static RobotManager manager;
        return manager;
    }
    RobotManager();
    void start();
    
    std::shared_ptr<Robot> getRobot(uint32 logicID)
    {
        return robots_[logicID];
    };
    void removeRobot(uint32 logicID)
    {
        robots_.erase(robots_.begin() + logicID);
    }
    
    void setRobotNum(int num) { robotNums_ = num; }
    
public:

    void postConnect(std::shared_ptr<Robot> robot);

private:
    
    int32 robotNums_;
    asio::io_context io_context_;
    std::vector<std::shared_ptr<Robot>> robots_;
    
    
    const std::string ip_;
    const uint16 port_;
};




