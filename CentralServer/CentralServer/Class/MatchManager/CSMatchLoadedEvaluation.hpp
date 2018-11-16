//
//  CSMatchLoadedEvaluation.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/15.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/CPGServerDefine.h>
#include <list>
#include <map>
#include "CSMatchDistDefine.h"

// 负载评估, 通过硬件设备
//生产环境： CPU占用率，内存占用率，带宽占用率评估。
// 预估未来负载: 通过分配的比赛，比赛类型，比赛时间，比赛其他信息等评估.
// v0.1： 测试  通过分配比赛数量评估
class CSMatchLoadedEvaluation
{
public:
    
    static int evaluate(int sid, const std::list<int>& mids);
    
    static void updateLoaded(int sid,
                    std::map<MatchDisService, std::list<int>>& services);
private:
    
};









