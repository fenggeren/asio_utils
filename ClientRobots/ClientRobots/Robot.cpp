//
//  Robot.cpp
//  ClientRobots
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "Robot.hpp"
#include "RobotManager.hpp"

void Robot::connect()
{
    gRobotManager.postConnect(shared_from_this());
}


const MatchBriefInfo& Robot::randMatch() const
{
    int index = arc4random_uniform(matchList_.size());
    assert(index < matchList_.size());
    return matchList_[index];
}
MatchBriefInfo Robot::matchInfo(int mid)
{
    auto iter = std::find_if(matchList_.begin(), matchList_.end(),
                 [&](const MatchBriefInfo& info)
                 {
                     return info.mid == mid;
                 });
    
    if (iter == matchList_.end())
    {
        printf("-- not found match mid: %d", mid);
        return {};
    }
    else
    {
        return *iter;
    }
}
void Robot::joinedMatch(int mid)
{
    joinedMatches_.insert(mid);
}
void Robot::unjoinedMatch(int mid)
{
    joinedMatches_.erase(mid);
}







