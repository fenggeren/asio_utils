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
#include <CPG/ErrorCodeDefine.h>
#include <CPG/GameDefine.h>
#include <CPG/CPGServer.pb.h>
#include <CPG/MatchHelper/CPGMatchStructHelper.hpp>
#include <CPG/CPGMatchDefine.h>
#include <algorithm>
#include "CSMatchDistribution.hpp"
#include "CSMatchLoadedEvaluation.hpp"

using namespace fasio::logging;

CSMatchManager::CSMatchManager()
:matchLatestUpdateTime_(time(NULL))
{
    DBConfig config{3306,"127.0.0.1","CPGLive",
        "root", "123456789"};
    dbwrapper_ = std::make_unique<DBActiveWrapper>(config, std::bind(&CSMatchManager::dbAsyncHandler, this, std::placeholders::_1));
}
// 初始化
void CSMatchManager::initialize()
{
    using namespace nlohmann;
    
    {
        CPGMatchCreateFactory factory;
        // 获取所有的比赛
        matches_ = factory.initialize();
    }
    
    std::list<int> allmatches;
    for(auto& pair : matches_)
    {
        allmatches.push_back(pair.first);
    }
    
    undistMatches_.swap(allmatches);
    
    // CS启动20s后自检一次，
    // 防止CS之前因为故障重新启动，
    // 20s内，用于其他服务连接，恢复分配的比赛信息.
    fasio::TimerManager::createTimer(
         std::bind(&CSMatchManager::updateDistMatchServices, this),
                                     fasio::getIoContext(), 20);
    
    dbwrapper_->start();
    // 已经分配的比赛
//    std::list<int> distedMatches;
//    for(auto& pair : matchServices_)
//    {
//        distedMatches.merge(pair.second);
//    }
}

std::list<int> CSMatchManager::getDistMatch(int sid)
{
    MatchDisService serviceKey{sid, 0};
    auto iter = matchServices_.find(serviceKey);
    if (iter != matchServices_.end())
    {
        return iter->second;
    }
    return {};
}

int CSMatchManager::getService(int mid)
{
    if (undistMatches_.size() > 0)
    {
        auto iter = std::find_if(undistMatches_.begin(),
                     undistMatches_.end(), [&](int undistmid)
                     {
                         return mid == undistmid;
                     });
        if (iter != undistMatches_.end())
        {
            return -1;
        }
    }
    
    for(auto& pair : matchServices_)
    {
        auto iter = std::find_if(pair.second.begin(),
                                 pair.second.end(), [&](int undistmid)
                                 {
                                     return mid == undistmid;
                                 });
        if (iter != pair.second.end())
        {
            return pair.first.sid;
        }
    }
    
    return -1;
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
        timer_ = std::make_shared<AsioTimer>(fasio::getIoContext());
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

void CSMatchManager::updateMatchesOrService()
{
    matchLatestUpdateTime_ = time(NULL);
    startTimerCheckDistMatchServices();
}

void CSMatchManager::addUndistriteMatches(std::list<int>&& mids)
{
    undistMatches_.merge(mids);
    undistMatches_.sort();
    undistMatches_.unique();
    updateMatchesOrService();
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
    updateMatchesOrService();
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
    updateMatchesOrService();
}

bool CSMatchManager::checkDistMatchServices()
{
    time_t now = time(NULL);
    // 时间间隔大于指定时间， 更新比赛分配信息
    return matchLatestUpdateTime_ - now >= updateDuration;
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
            auto iter = matchServices_.find(srv.service);
            if (iter == matchServices_.end() ||
                iter->second.size() == 0)
            {
                matchServices_[srv.service] = {};
            }
            else //
            {
                LOG_ERROR << "add service has exist: " << srv.service.sid
                << "  " << jointContainer(iter->second);
            }
        }
        
       
        // ④ 策略重新分配 比赛<=>服务
        
        auto changedServersList = MatchDistribution()(matchServices_, matches_,undistMatches_);
        ChangedMatchMap changedMap;
        for(auto sid : changedServersList)
        {
            changedMap[sid] = getDistMatch(sid);
        }
        
        // 更新负载
        
        
        // 清除
        // 后期可以根据负载，发送警报给运维/自动化，启动更多MS,已降低负载。
        changedServices_.clear();
        // 还存在为分配的比赛
        if (undistriMatches.size() > 0)
        {
            startTimerCheckDistMatchServices();
        }
        
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
CSMatchManager::CheckError
CSMatchManager::checkServiceDistMap(const MatchDisService& service,
                                         std::list<int>& mids)
{
    // 校验 mids是否都有效，祛除无效的mids
    {
        auto invalidMids = filterValidMatch(mids);
        if (invalidMids.size() > 0)
        {
            LOG_MINFO << "matchserver check invalid  sid: " << service.sid
            << " invalidmids: " << jointContainer(invalidMids);
        }
    }
    
    
    CheckError res = ErrorNo;
    auto iter = matchServices_.find(service);
    if (iter != matchServices_.end())
    {
        {
            auto invalidMids = filterValidMatch(mids);
            if (invalidMids.size() > 0)
            {
                LOG_MINFO << "CentralServer check invalid  sid: "
                << service.sid
                << " invalidmids: " << jointContainer(invalidMids);
            }
        }
        // 判断 交集
        auto& distMids = iter->second;
        distMids.sort();
        mids.sort();
        
        std::list<int> diff;
        std::set_difference(distMids.begin(), distMids.end(),
                            mids.begin(), mids.end(),
                            std::inserter(diff, diff.begin()));
        
        // 应该把多余的比赛，放入undist中
        if (diff.size() > 0)
        {
            std::string mids;
            for (auto mid : diff)
            {
                mids += (std::to_string(mid) + ", " );
            }
            LOG_ERROR << " MatchService: " << service.sid
            << " lack mids: " << mids;
            res = ErrorMatchServerLess;
        }
        
        // 返回给ms，新的分配比赛
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
            res = ErrorMatchServerMore;
        }
    }
    //
    else if (mids.size() > 0)
    {
        res = ErrorReconnect;
        matchServices_[service] = mids;
        for(auto& mid : mids)
        {
            undistMatches_.remove(mid);
        }
        
        LOG_MINFO << " reconnect recover " << service.sid
        << " : " <<jointContainer(matchServices_[service]) ;
        
        // CS重启，MS自动重连的情况。
        // 需要清理 changedServices_
        changedServices_.remove_if([=](const ChangedService& changed)
        {
            return changed.service.sid == service.sid;
        });
    }
    else
    {
        res = ErrorNoMatch;
        // 已经0负载。
        updateMatchesOrService();
    }
    
    return res;
}


std::list<int>
CSMatchManager::filterValidMatch(std::list<int>& mids)
{
    return {};
}

std::list<std::shared_ptr<CPGMatchProfile>>
CSMatchManager::matchList() const
{
    std::list<std::shared_ptr<CPGMatchProfile>> list;
    
    for(auto& pair : matches_)
    {
        list.push_back(pair.second);
    }
    
    return list;
}


// 取消比赛
int CSMatchManager::cancelMatch(int mid,int reason)
{
    auto match = getMatch(mid);
    if (!match)
    {
        return kManager_MatchNotFound;
    }
    if (match->match_state > ENUM_STATE_READY)
    {
        return kManager_MatchCancel_StartedError;
    }
    
    // 移除本地数据。
    int sid = getService(mid);
    if (sid > 0) // 已分配，需要移除多处数据，并将比赛更新分配信息通知出去.
    {
        MatchDisService serviceKey{sid, 0};
        auto iter = matchServices_.find(serviceKey);
        if (iter != matchServices_.end())
        {
            iter->second.remove(mid);
            // 通知其他服务.
            ChangedMatchMap changedMap;
            changedMap[sid] = iter->second;
            changedMatchMapCB_(changedMap);
        }
    }
    undistMatches_.remove(mid);
    
    CPGServer::CancelMatchRQ rq;
    rq.set_mid(mid);
    rq.set_reason(reason);
    dbwrapper_->sendToDBThread(rq.SerializeAsString().data(),
                               {kCancelMatchRQ, rq.ByteSize(),0});
    
    return kNoneError;
}
// 更新比赛参数.
int CSMatchManager::updateMatch(int mid, const std::map<std::string,std::string>& properties)
{
    // 更新本地数据
    auto match = getMatch(mid);
    if (match == nullptr)
    {
        return kManager_MatchNotFound;
    }
    // 更新local
    auto matchproperties = structConvertMap(*match.get());
    for(auto& pair : properties)
    {
        matchproperties[pair.first] = pair.second;
    }
    mapConvertStruct(matchproperties, *match.get());
    
    // 更新数据库
    CPGServer::UpdateMatchRQ rq;
    
    for(auto& pro : properties)
    {
        auto p = rq.mutable_properties();
        
        p->insert({pro.first, pro.second});
    }
    dbwrapper_->sendToDBThread(rq.SerializeAsString().data(),
                               {kUpdateMatchRQ, rq.ByteSize(),0});
    return kNoneError;
}
// 获取比赛
std::shared_ptr<CPGMatchProfile>
CSMatchManager::getMatch(int mid)
{
    auto iter = matches_.find(mid);
    if (iter != matches_.end()) {
        return iter->second;
    }
    
    return nullptr;
}
// 创建比赛, mid = 0, 存储到mysql,获取唯一的mid
int CSMatchManager::createMatch(const CPGMatchProfile& match, int promiseID)
{
    int code = createMatchValid(match);
    if (code != kNoneError)
    {
        return code;
    }
    
    // 检测比赛信息有效性
    CPGServer::CreateMatchRQ rq;
    auto properties = structConvertMap(match);
    auto p = rq.mutable_properties();
    for(auto& pro : properties)
    {
        p->insert({pro.first, pro.second});
    }
     
    dbwrapper_->sendToDBThread(rq.SerializeAsString().data(),
                               {kCreateMatchRQ, rq.ByteSize(),promiseID});
    
    
    return code;
}

void createMatchWait(const CPGMatchProfile& match,
                     std::promise<int>&& promise)
{
    
}


void CSMatchManager::createdMatches(const   std::list<std::shared_ptr<CPGMatchProfile>>& matches)
{
    std::list<int> mids;
    for(auto& match : matches)
    {
        matches_[match->mid] = match;
        mids.push_back(match->mid);
    }
    addUndistriteMatches(std::move(mids));
}

int CSMatchManager::createMatchValid(const CPGMatchProfile& match)
{
    time_t cur = time(NULL);
    if (match.start_time < cur + 60)
    {
        return kCreateMatch_ErrorTimeOut;
    }
    
    // 统一类型的比赛，统一时间只会开赛一场
    for(auto& pair : matches_)
    {
        if (pair.second->start_time == match.start_time &&
            pair.second->match_type == match.match_type &&
            pair.second->entryfee == match.entryfee)
        {
            return kCreateMatch_ErrorHasExist;
        }
    }
    return kNoneError;
}







