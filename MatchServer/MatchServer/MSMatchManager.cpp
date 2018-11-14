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
}
