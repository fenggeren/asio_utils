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
#include <list>
#include <vector>
#include <set>
#include "StructDefine.h"
#include "BeatHeartClient.hpp"

class Robot : public std::enable_shared_from_this<Robot>
{
public:
    
    Robot(const std::string& ip, uint16 port)
    : port_(port), ip_(ip)
    {
    }
    
    void connect();
    
    void joinedMatch(int mid);
    void unjoinedMatch(int mid);
    
public:
    
    void setUid(int uid)
    {
        userInfo_.uid = uid;
    }
    void setSession(const std::shared_ptr<fasio::ClientSession>& session)
    {
        userInfo_.session = session;
    }
    
    void setUserInfo(const UserInfo& info)
    {
        userInfo_ = info;
    }
    const UserInfo& userInfo() const { return userInfo_; }
    
    const MatchBriefInfo& randMatch() const;
    
    void setMatches(std::vector<MatchBriefInfo>&& matches)
    {
        matchList_.swap(matches);
    }
    
    MatchBriefInfo matchInfo(int mid);
   
    
public:
    const std::string& ip() const {return ip_; }
    void setIP(const std::string& ip) { ip_ = ip; }
    uint16 port() const { return port_;}
    void setPort(uint16 port) { port_ = port; }

    uint32 logicID() const { return logicID_; }
    void setLogicID(uint32 logicID) { logicID_ = logicID; }
private:
    unsigned short port_;
    std::string ip_;
    uint32 logicID_;
    UserInfo    userInfo_;
    std::vector<MatchBriefInfo> matchList_;
    std::set<int>  joinedMatches_;
};



