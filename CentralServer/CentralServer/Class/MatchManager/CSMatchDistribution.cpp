//
//  CSMatchDistribution.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/13.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSMatchDistribution.hpp"
#include "cpg_match_create_factory.h"
#include <algorithm>
#include <vector>
#include <Net/logging/Logging.hpp>
#include "CSMatchLoadedEvaluation.hpp"

using namespace fasio::logging;

std::list<unsigned int>
MatchDistribution::operator()(
        std::map<MatchDisService, std::list<int>>& matchServices,
           std::list<int>& undistMatches)
{
    if (matchServices.size() == 0)
    {
        return {};
    }
    auto matches = cpg_match_create_factory::instance().get_all_matches();
    auto allLoaded = matches.size();
    
    auto allServices = matchServices.size();
    
    int countPerService = floor(allLoaded / (float)allServices);
    
    std::set<int> changed;
    auto tmp = distributeFill(matchServices, countPerService, undistMatches);
    changed.insert(tmp.begin(), tmp.end());
    
    tmp = distributeClipping(matchServices, countPerService, undistMatches);
    changed.insert(tmp.begin(), tmp.end());
    
    if (undistMatches.size() > 0)
    {
        auto tmp = distributeFill(matchServices, countPerService, undistMatches);
        changed.insert(tmp.begin(), tmp.end());
    }
    std::list<unsigned int> redistributedMids;
    redistributedMids.assign(changed.begin(), changed.end());
    return redistributedMids;
}


// 填谷
std::set<int>
MatchDistribution::distributeFill(
            std::map<MatchDisService, std::list<int>>& matchServices,
               int averageLoaded,
               std::list<int>& undistMatches)
{
    std::set<int> changedServices;
    
    // 负载排序 小到大排序
    std::list<MatchDisService> ordered;
    for(auto& ms : matchServices)
    {
        ordered.push_back(ms.first);
    }
    ordered.sort([](const MatchDisService& vl, const MatchDisService& vr)
                 {
                     return vl.loaded < vr.loaded;
                 });
    
    for(auto& srv : ordered)
    {
        if (undistMatches.size() <= 0) break;
        // 计算负载负载差额！  TODO. 比赛类型&服务器配置
        int diff = averageLoaded - srv.loaded;
        if (diff <= 0) { break; }
        
        auto iter = matchServices.find(srv);
        for(int i = 0; i < diff; i++)
        {
            int mid = undistMatches.front();
            undistMatches.pop_front();
            iter->second.push_back(mid);
        }
        LOG_DEBUG << " service sid: " << iter->first.sid
        << " count: " << iter->second.size();
        changedServices.insert(srv.sid);
    }
    
    
    // 更新负载
    for(auto& cid : changedServices)
    {
        CSMatchLoadedEvaluation::updateLoaded(cid, matchServices);
    }
    
    return changedServices;
}

// 消峰
std::set<int>
MatchDistribution::distributeClipping(
                std::map<MatchDisService, std::list<int>>& matchServices,
                   int averageLoaded,
                   std::list<int>& undistMatches)
{
    std::set<int> changedServices;
    
    // 负载排序 小到大排序
    std::list<MatchDisService> ordered;
    for(auto& ms : matchServices)
    {
        ordered.push_back(ms.first);
    }

    ordered.sort([](const MatchDisService& vl, const MatchDisService& vr)
                 {
                     return vl.loaded > vr.loaded;
                 });
    
    
    // 最小负载 < 指定指标， 或者最大负载 > 大指标。
    if (!(tooFree(ordered.back()) || tooBusy(ordered.front())))
    {
        return {};
    }
    
    for(auto& srv : ordered)
    {
        // 计算负载负载差额！  TODO. 比赛类型&服务器配置
        int diff = srv.loaded - averageLoaded;
        if (diff <= 0) { break; }
        
        auto iter = matchServices.find(srv);
        auto& mes = iter->second;
        
        // 指向下一个 比赛id，
        auto changedIter = mes.begin();
        
        bool changed = false;
        
        // 重新分配diff个比赛
        for (int i = 0; i < diff && changedIter != mes.end(); i++)
        {
            do
            {
                // 可以重新分配
                if (canRedistribute(*changedIter))
                {
                    undistMatches.push_back(*changedIter);
                    changedIter = mes.erase(changedIter);
                    changed = true;
                    break;
                }
                else
                {
                    changedIter++;
                }
            } while (true);
            // 进行未开始判断!
        }
        
        if (changed)
        {
            changedServices.insert(srv.sid);
        }
    }
    
    // 更新负载
    for(auto& cid : changedServices)
    {
        CSMatchLoadedEvaluation::updateLoaded(cid, matchServices);
    }
    
    return changedServices;
}


// 根据硬件 & 当前运行比赛 预估负载， 判断
bool MatchDistribution::tooFree(const MatchDisService& srv)
{
    return srv.loaded <= 0;
}

bool MatchDistribution::tooBusy(const MatchDisService& srv)
{
    return srv.loaded >= 10;
}

bool MatchDistribution::canRedistribute(int mid)
{
    return true;
}
