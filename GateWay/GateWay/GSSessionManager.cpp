//
//  GSSessionManager.cpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "GSSessionManager.hpp"
#include <Net/logging/Logging.hpp>
using namespace fasio::logging;


void GSSessionManager::transToMatchServer(uint32 mid, const void* data, uint32 len)
{
    // 根据比赛 mid 获取对应的 G2MSession!
    auto iter = mid2MatchServers_.find(mid);
    if (iter != mid2MatchServers_.end())
    {
        
    }
    else
    {
        LOG_ERROR << " not found matchserver for mid: " << mid;
    }
}
