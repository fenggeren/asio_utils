//
//  CSMatchLoadedEvaluation.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/15.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSMatchLoadedEvaluation.hpp"



int
CSMatchLoadedEvaluation::evaluate(int sid, const std::list<int>& mids)
{
    return mids.size();
}

void CSMatchLoadedEvaluation::updateLoaded(int sid,
                         std::map<MatchDisService, std::list<int>>& services)
{
    auto iter = services.find({sid, 0});
    int loaded = evaluate(sid, iter->second);
    
    std::list<int> mids(std::move(iter->second));
    services.erase(iter);
    services.insert({{sid, loaded}, std::move(mids)});
}














