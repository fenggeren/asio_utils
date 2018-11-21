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

using namespace fasio;

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

    
    void joinMatchRQ(const std::shared_ptr<Robot>& robot);
    void unjoinMatchRQ(const std::shared_ptr<Robot>& robot);
    void matchListRQ(const std::shared_ptr<Robot>& robot);
    
    
    void loginRS(const std::shared_ptr<TCPSession>& session,
                 const void* data, const PacketHeader& heaer);
    void joinMatchRS(const std::shared_ptr<TCPSession>& session,
                     const void* data, const PacketHeader& heaer);
    void unjoinMatchRS(const std::shared_ptr<TCPSession>& session,
                       const void* data, const PacketHeader& heaer);
    void matchListRS(const std::shared_ptr<TCPSession>& session,
                     const void* data, const PacketHeader& heaer);
    
    void heartBeat(const std::shared_ptr<Robot>& robot);
    void heartReatReceive(const std::shared_ptr<TCPSession>& session,
                          const void* data, const PacketHeader& heaer);
    
    
    void receiveData(const std::shared_ptr<TCPSession>& session);
private:
    
    std::shared_ptr<Robot> getRobot(const std::shared_ptr<TCPSession>& session);
    
private:
    
    int32 robotNums_;
    asio::io_context io_context_;
    std::vector<std::shared_ptr<Robot>> robots_;
    
    const std::string ip_;
    const uint16 port_;
};




