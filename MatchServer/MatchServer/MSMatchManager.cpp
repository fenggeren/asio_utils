//
//  MSMatchManager.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/14.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "MSMatchManager.hpp"


void MSMatchManager::updateMatches(const std::list<int> matches)
{
    matches_ = matches;
    std::map<int, int> tmp;
    for(auto mid : matches)
    {
        tmp[mid] = testMatchinfos_[mid];
    }
    tmp.swap(testMatchinfos_);
}

int MSMatchManager::joinMatch(int uid, int mid, int mtype)
{
    auto iter = testMatchinfos_.find(mid);
    if (iter != testMatchinfos_.end())
    {
        testMatchinfos_[mid]++;
        return testMatchinfos_[mid];
    }
    else
    {
        return -1;
    }
}
int MSMatchManager::unjoinMatch(int uid, int mid, int mtype)
{
    auto iter = testMatchinfos_.find(mid);
    if (iter != testMatchinfos_.end())
    {
        testMatchinfos_[mid]--;
        return testMatchinfos_[mid];
    }
    else
    {
        return -1;
    }
}




