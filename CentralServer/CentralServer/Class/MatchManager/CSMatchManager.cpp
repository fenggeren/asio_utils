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
    
    
    // 获取所有的比赛
    auto& matches = factory.get_all_matches();
    std::list<int> allmatches;
    for(auto& pair : matches)
    {
        allmatches.push_back(pair.first);
    }
    
    undistMatches_ = std::move(allmatches);
    
    // CS启动20s后自检一次，
    // 防止CS之前因为故障重新启动，
    // 20s内，用于其他服务连接，恢复分配的比赛信息.
    fasio::TimerManager::createTimer(
         std::bind(&CSMatchManager::updateDistMatchServices, this),
                                     fasio::getIoContext(MAIN), 20);
    // 已经分配的比赛
//    std::list<int> distedMatches;
//    for(auto& pair : matchServices_)
//    {
//        distedMatches.merge(pair.second);
//    }
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
            if (!ec)
            {
                updateDistMatchServices();
            }
            else
            {
                LOG_DEBUG << " start timer block: " << ec.value();
            }
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

void CSMatchManager::addUndistriteMatches(std::list<int>&& mids)
{
    undistMatches_.merge(mids);
    undistMatches_.sort();
    undistMatches_.unique();
    startTimerCheckDistMatchServices();
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
    LOG_DEBUG << "";
    stopTimerCheckDistMatchServices();
    if (checkDistMatchServices())
    {
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

// 基准！！！
// 只有CS有分配比赛的功能
// 其余service不能删掉，添加比赛。
bool CSMatchManager::checkServiceDistMap(const MatchDisService& service,
                                         std::list<int>& mids)
{
    bool res = true;
    auto iter = matchServices_.find(service);
    if (iter != matchServices_.end())
    {
        // 判断 交集
        auto& distMids = iter->second;
        distMids.sort();
        mids.sort();
        
        std::list<int> diff;
        std::set_difference(distMids.begin(), distMids.end(),
                            mids.begin(), mids.end(),
                            std::inserter(diff, diff.begin()));
        
        if (diff.size() > 0)
        {
            std::string mids;
            for (auto mid : diff)
            {
                mids += (std::to_string(mid) + ", " );
            }
            LOG_ERROR << " MatchService: " << service.sid
            << " lack mids: " << mids;
            res = false;
        }
        
        //
        diff.clear();
        std::set_difference(mids.begin(), mids.end(),
                            distMids.begin(), distMids.end(),
                            std::inserter(diff, diff.begin()));
        if (diff.size() > 0)
        {
            std::string mids;
            for (auto mid : diff)
            {
                mids += (std::to_string(mid) + ", " );
            }
            LOG_ERROR << " MatchService: " << service.sid
            << " more mids: " << mids;
            res = false;
        }
    }
    else if (mids.size() > 0)
    {
        std::string midStr;
        matchServices_[service] = mids;
        for(auto& mid : mids)
        {
            midStr += (std::to_string(mid) + ", " );
            undistMatches_.remove(mid);
        }
        
        LOG_MINFO << " reconnect recover: " << midStr;
    }
    else
    {
        // 已经0负载。
        startTimerCheckDistMatchServices();
    }
    
    return res;
}












