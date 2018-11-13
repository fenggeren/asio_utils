//
//  CSMatchManager.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSMatchManager.hpp"
#include <string>
#include <iostream>
#include <Net/Conv.hpp>
#include <Net/logging/Logging.hpp>
#include <algorithm>
#include "cpg_match_create_factory.h"
#include "CSMatchDistribution.hpp"

using namespace fasio::logging;


// 初始化
void CSMatchManager::initialize()
{
    using namespace nlohmann;
    
    auto& factory = cpg_match_create_factory::instance();
    factory.load_from_file();
    factory.create_all_matches();
}

std::list<int> CSMatchManager::getDistMatch(unsigned int sid)
{
    MatchDisService serviceKey{sid, 0};
    auto iter = matchServices_.find(serviceKey);
    if (iter != matchServices_.end())
    {
        return iter->second;
    }
    return {};
}

// 加载所有比赛
void CSMatchManager::loadAllMatches()
{
    
}
// 更新所有配置比赛
void CSMatchManager::updateConfigMatches()
{
    
}

void CSMatchManager::startTimerCheckDistMatchServices()
{
    if (timer_ == nullptr)
    {
        timer_ = std::make_shared<AsioTimer>(fasio::getIoContext(MAIN));
        timer_->expires_from_now(std::chrono::milliseconds(S2M(10)));
        timer_->async_wait([&](std::error_code ec){
            updateDistMatchServices();
        });
    }
}
void CSMatchManager::stopTimerCheckDistMatchServices()
{
    if (timer_)
    {
        timer_->cancel();
        timer_ = nullptr;
    }
}

void
CSMatchManager::updateMatchService(const std::shared_ptr<ServerInfo>& service)
{
    MatchDisService serviceKey{service->sid, service->loaded};
    ChangedService::Type type = ChangedService::Update;
    
    if (matchServices_.find(serviceKey) == matchServices_.end())
    {
        type = ChangedService::Added;
    }
    
    for(auto changed = changedServices_.begin();
        changed != changedServices_.end();)
    {
        if (changed->service.sid == serviceKey.sid)
        {
            LOG_MINFO << " changedServices has  had this service: "
            << changed->service.sid;
            changed = changedServices_.erase(changed);
        }
        else
        {
            changed++;
        }
    }
    
    ChangedService changedService{
        type,
        serviceKey,
        time(NULL)
    };
    
    changedServices_.push_back(std::move(changedService));
    updateDistMatchServices();
}

void
CSMatchManager::removeMatchService(const std::shared_ptr<ServerInfo>& service)
{
    MatchDisService serviceKey{service->sid, service->loaded};
 
    bool requireAdded = true;

    for(auto changed = changedServices_.begin();
        changed != changedServices_.end();)
    {
        if (changed->service.sid == serviceKey.sid)
        {
           if (changed->type == ChangedService::Added)
            {
                // 如果该服务刚连接上，就失联了<指定时间内>，直接移除，
                // 不触发分配比赛逻辑。
                changed = changedServices_.erase(changed);
                requireAdded = false;
            }
            else //其他情况， 进行覆盖操作
            {
                changed = changedServices_.erase(changed);
            }
        }
        else
        {
            changed++;
        }
    }
    
    if (requireAdded)
    {
        ChangedService changedService{
            ChangedService::Removed,
            serviceKey,
            time(NULL) };
        changedServices_.push_back(std::move(changedService));
    }
    updateDistMatchServices();
}

bool CSMatchManager::checkDistMatchServices()
{
    time_t now = time(NULL);
    time_t before = now;
    time_t after = now;
    
    for (auto& changed : changedServices_)
    {
        if (changed.stamp > after)
        {
            after = changed.stamp;
        }
        else if (changed.stamp < before)
        {
            before = changed.stamp;
        }
    }
    
    // 时间间隔大于指定时间， 更新比赛分配信息
    if (after - before >= updateDuration)
    {
        return true;
    }
    else
    {
        if (changedServices_.size() == 0 &&
            undistMatches_.size() > 0)
        {
            return true;
        }
    }
    return false;
}

// 已经进行处理， changedServices_ 每个sid，只包含一个改变.
void CSMatchManager::updateDistMatchServices()
{
    if (checkDistMatchServices())
    {
        stopTimerCheckDistMatchServices();
        
        //① 移除掉，断线的服务，将分配给它的比赛，存储如undistMatches_中
        std::list<int> undistriMatches;
        std::list<ChangedService> removedServices;
        changedServices_.remove_if([&](const ChangedService& changed)
        {
            if (changed.type == ChangedService::Removed)
            {
                undistriMatches.merge(
                 std::forward<std::list<int>>(
                 getDistMatch(changed.service.sid)));
                matchServices_.erase(changed.service);
            }
            removedServices.push_back(changed);
            return changed.type == ChangedService::Removed;
        });
        undistMatches_.merge(undistriMatches);
        
        //② 将更新的服务，进行信息更新!
        changedServices_.remove_if([&](const ChangedService& changed)
        {
           if (changed.type == ChangedService::Update)
           {
               auto iter = matchServices_.find(changed.service);
               if (iter == matchServices_.end())
               {
                   LOG_ERROR << "cant found update srv: " << changed.service.sid;
               }
               else
               {
                   auto pair = *iter;
                   matchServices_.erase(iter);
                   matchServices_.insert({changed.service, pair.second});
               }
               return true;
           }
            return false;
        });
        
        // ③
        for (auto& srv : changedServices_)
        {
            matchServices_[srv.service] = {};
        }
        
       
        // ④ 策略重新分配 比赛<=>服务
        
        auto changedServersList = MatchDistribution()(matchServices_, undistMatches_);
        ChangedMatchMap changedMap;
        for(auto sid : changedServersList)
        {
            changedMap[sid] = getDistMatch(sid);
        }
        // 清除
        // 后期可以根据负载，发送警报给运维/自动化，启动更多MS,已降低负载。
        changedServices_.clear();
        undistMatches_.clear();
        
        // 调用更新回调
        if (changedMatchMapCB_)
        {
            changedMatchMapCB_(changedMap);
        }
    }
    else
    {
        startTimerCheckDistMatchServices();
    }
}













