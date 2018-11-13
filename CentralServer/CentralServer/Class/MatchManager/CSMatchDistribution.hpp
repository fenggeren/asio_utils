//
//  CSMatchDistribution.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/13.
//  Copyright © 2018年 fgr. All rights reserved.
//
#pragma once

#include <map>
#include <list>
#include <set>
#include "CSMatchDistDefine.h"

class MatchDistribution
{
public:

    /*
     将高负载的服务，移除掉部分未开赛比赛到 undistMatches中，以便集中分配.
     初期： 一个比赛+1负载
     */
    std::list<unsigned int>
    operator()(std::map<MatchDisService, std::list<int>>& matchServices, 
               std::list<int>& undistMatches);
    
private:
    
    // 填谷
    std::set<int>
    distributeFill(std::map<MatchDisService, std::list<int>>& matchServices,
                                int averageLoaded,
                  std::list<int>& undistMatches);
    // 消峰
    std::set<int>
    distributeClipping(std::map<MatchDisService, std::list<int>>& matchServices,
                  int averageLoaded,
                  std::list<int>& undistMatches);
    
    
    // 给定服务是否太忙/闲？
    bool tooFree(const MatchDisService& srv);
    bool tooBusy(const MatchDisService& srv);
    
    // 比赛是否可以重新分配 运行服务
    bool canRedistribute(int mid);
private:
    int averageLoaded_{0};
};












