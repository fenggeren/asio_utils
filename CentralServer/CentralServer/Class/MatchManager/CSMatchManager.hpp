//
//  CSMatchManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <CPG/CPGServerDefine.h>
#include <Net/TimerManager.hpp>
#include <map>
#include <list>
#include "CSMatchDistDefine.h"



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
    
    std::map<MatchDisService, std::list<int>>
    getAllMatchServices();
    
    std::list<int> getDistMatch(unsigned int sid);
public:
    
    // 启动/暂停新的比赛服务.
    // 可能需要从新分配比赛
    // 都应该具有滞后性，即调用后，延时更新比赛对应服务配置表。
    // 延时长短，根据比赛信息具体确定！
    // 有可能是 重新连接的服务，这里需要特殊处理
    // @return  返回重新分配的比赛服务对应信息。 不改变的比赛无需返回
    void
    updateMatchService(const std::shared_ptr<ServerInfo>& service);
    void
    removeMatchService(const std::shared_ptr<ServerInfo>& service);
    
    
private:
    // 加载所有比赛
    void loadAllMatches();
    // 更新所有配置比赛
    void updateConfigMatches();
    
    // 更新比赛服务分配表
    // 评估，是否有些服务，负载过高!
    bool checkDistMatchServices();
    void updateDistMatchServices();
    
    void startTimerCheckDistMatchServices();
    void stopTimerCheckDistMatchServices();
    
private:
    // 异步更新 比赛对应服务表--
    ChangedMatchMapCB changedMatchMapCB_;
    // 所有的比赛服务 对应的 所运行比赛id
    std::map<MatchDisService, std::list<int>> matchServices_;
    
    
    std::list<ChangedService> changedServices_;
    // 比赛开始前1分钟？分配比赛？ CS会是流量瓶颈?
    std::list<int> undistMatches_; // 暂未分配的比赛。
    // 比赛对应表 滞后更新
    const time_t updateDuration = 60;
    
    using AsioTimer = asio::basic_waitable_timer<std::chrono::steady_clock>;
    std::shared_ptr<AsioTimer> timer_;
};











