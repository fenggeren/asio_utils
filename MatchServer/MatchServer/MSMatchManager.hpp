//
//  MSMatchManager.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/14.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <map>
#include <list>


class MSMatchManager
{
public:
    
    static MSMatchManager& instance()
    {
        static MSMatchManager manager;
        return manager;
    }
    
    void updateMatches(const std::list<int> matches);
    
    
    int joinMatch(int uid, int mid, int mtype);
    int unjoinMatch(int uid, int mid, int mtype);
    
public:
    std::list<int> matches() const {  return matches_; }
private:
    
//    std::map<int, std::shared_ptr<cpg_match_info>> matches_;
    
    std::list<int> matches_;
    
    // <mid, allplayer>
    std::map<int, int> testMatchinfos_;
};




