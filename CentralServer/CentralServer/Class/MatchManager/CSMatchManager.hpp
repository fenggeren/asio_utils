//
//  CSMatchManager.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <CPG/CPGServerDefine.h>
#include <CPG/Util/ConvFunctional.hpp>
#include <Net/TimerManager.hpp>
#include <map>
#include <list>
#include "CSMatchDistDefine.h"
#include <CPG/MatchCreate/CPGMatchCreateFactory.hpp>


/*
 ① CS重启
    其他服务直接重连，需要recover分配的比赛信息。
 ② MS重启
    连接MS， 重新分配比赛信息。
 
 心跳包机制，判断某个MS已经失联。 相关分配策略。
 */

// 负责管理所有比赛
/*
 如果CS崩溃重启？
 其他服务自动重连。
 MS将自己分配的所有比赛发送给CS
 */
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
    getAllMatchServices()
    {   return matchServices_; }
    
    std::list<std::shared_ptr<CPGMatchProfile>>
    matchList() const;
    
    std::list<int> getDistMatch(int sid);
    
    void addUndistriteMatches(std::list<int>&& mids);
public:
    // 启动/暂停新的比赛服务.
    // 可能需要从新分配比赛
    // 都应该具有滞后性，即调用后，延时更新比赛对应服务配置表。
    // 延时长短，根据比赛信息具体确定！
    // 有可能是 重新连接的服务，这里需要特殊处理
    // @return  返回重新分配的比赛服务对应信息。 不改变的比赛无需返回
    void
    updateMatchService(const std::shared_ptr<ServerInfo>& service);
    // 判断比赛严重性！
    // 如果有MS运行的比赛已经开赛，需要重新评估
    // 重新分配该比赛，重新开赛？<崩溃故障还是网络故障>
    void
    removeMatchService(const std::shared_ptr<ServerInfo>& service);
    
    
    enum CheckError
    {
        ErrorNo = 0,
        ErrorMatchServerMore = 1,  // MS有多的比赛
        ErrorMatchServerLess = 2, // MS 缺少分配的比赛
        ErrorNoMatch = 3, // 没有比赛
        ErrorReconnect = 4, // 重新连接
    };
    // 校验 比赛分配信息
    CheckError checkServiceDistMap(const MatchDisService& service,
                             std::list<int>& mids);
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
    
    // 过滤比赛有效性
    // @return返回无效的比赛
    std::list<int>
    filterValidMatch(std::list<int>& mids);
    
private:
    // 异步更新 比赛对应服务表--
    ChangedMatchMapCB changedMatchMapCB_;
    // 所有的比赛服务 对应的 所运行比赛id
    std::map<MatchDisService, std::list<int>> matchServices_;
    
    std::map<int, std::shared_ptr<CPGMatchProfile>> matches_;
    
    std::list<ChangedService> changedServices_;
    // 比赛开始前1分钟？分配比赛？ CS会是流量瓶颈?
    std::list<int> undistMatches_; // 暂未分配的比赛。
    // 比赛对应表 滞后更新
    const time_t updateDuration = 25;
    
    using AsioTimer = asio::basic_waitable_timer<std::chrono::steady_clock>;
    std::shared_ptr<AsioTimer> timer_;
};











