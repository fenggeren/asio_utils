//
//  WSMatchManager.hpp
//  WebServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/MatchHelper/CPGMatchCreateFactory.hpp>

class WSMatchManager
{
public:
    
    WSMatchManager();
    
    // 全部是数据库操作
    
    // 创建比赛
    // 更新比赛
    // 取消比赛
    // 查询比赛列表
    // 比赛玩家状态
    
private:
    CPGMatchCreateFactory factory;
};



