//
//  Robot.hpp
//  ClientRobots
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//


#pragma once
#include <string>
#include <Net/base/FASIOType.h>

class Robot : public std::enable_shared_from_this<Robot>
{
public:
    
    Robot(const std::string& ip, uint16 port)
    : port_(port), ip_(ip)
    {
    }
    
    void connect();
 

public:
    const std::string& ip() const {return ip_; }
    void setIP(const std::string& ip) { ip_ = ip; }
    uint16 port() const { return port_;}
    void setPort(uint16 port) { port_ = port; }

    uint32 sessionID() const { return sessionID_; }
    void setSessionID(uint32 sessionID) { sessionID_ = sessionID_; }
private:
    unsigned short port_;
    std::string ip_;
    uint32 sessionID_;
};



