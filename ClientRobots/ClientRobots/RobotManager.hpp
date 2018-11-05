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
#include <unordered_map>
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
    
    std::shared_ptr<Robot> getRobot(uint32 logicID) const
    {
        return robotsMap_[logicID];
    };
    void removeRobot(uint32 logicID)
    {
        robotsMap_.erase(logicID);
    }

public:

    void postConnect(std::shared_ptr<Robot> robot);

private:
    
    int32 robotNums_;
    asio::io_context io_context_;
    std::unordered_map<uint32, std::shared_ptr<Robot>> robotsMap_;

    const std::string ip_;
    const uint16 port_;
};




