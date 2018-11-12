//
//  CSMatchManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include "cpg_match_create_factory.h"
#include <CPG/CPGServerDefine.h>
#include <map>

struct MatchDisService
{
    unsigned int        sid{0};
    unsigned long long  loaded{0};
};


// 负责管理所有比赛
class CSMatchManager
{
    using ChangedMatchMap = std::map<unsigned int, std::list<int>>;
    using ChangedMatchMapCB = std::function<void(const ChangedMatchMap& map)>;
public:
    
    static CSMatchManager& instance()
    {
        static CSMatchManager manager;
        return manager;
    }
    
    // 初始化
    void initialize();
    
    void setChangedMatchMapCB(ChangedMatchMapCB&& cb)
    {
        changedMatchMapCB_ = cb;
    }
    
public:
    
    // 启动/暂停新的比赛服务.
    // 可能需要从新分配比赛
    // 都应该具有滞后性，即调用后，延时更新比赛对应服务配置表。
    // 延时长短，根据比赛信息具体确定！
    // @return  返回重新分配的比赛服务对应信息。 不改变的比赛无需返回
    void
    updateMatchService(const std::shared_ptr<ServerInfo>& service);
    void
    removeMatchService(int sid);
    
private:
    // 加载所有比赛
    void loadAllMatches();
    // 更新所有配置比赛
    void updateConfigMatches();
private:
    
    cpg_match_create_factory match_factory_;
    // 异步更新 比赛对应服务表--
    ChangedMatchMapCB changedMatchMapCB_;
    // 所有的比赛服务 对应的 所运行比赛id
    std::map<MatchDisService, std::list<int>> matchServices_;
};











