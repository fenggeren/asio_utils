//
//  CSMatchManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include "cpg_match_create_factory.h"

// 负责管理所有比赛
class CSMatchManager
{
public:
    
    static CSMatchManager& instance()
    {
        static CSMatchManager manager;
        return manager;
    }
    
    // 初始化
    void initialize();
    

    
private:
    // 加载所有比赛
    void loadAllMatches();
    // 更新所有配置比赛
    void updateConfigMatches();
private:
    cpg_match_create_factory match_factory_;
};

