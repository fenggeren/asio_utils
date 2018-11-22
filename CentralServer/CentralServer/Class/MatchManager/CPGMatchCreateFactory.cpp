//
//  CPGMatchCreateFactory.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CPGMatchCreateFactory.hpp"
#include <CPG/Third/json/json.hpp>
#include <Net/FASIO.hpp>
#include "BetAlgorithm.h"

using namespace fasio::logging;

std::list<CPGMatchProfile> CPGMatchCreateFactory::loadFromDB()
{
    return {};
}

// json 创建
bool CPGMatchCreateFactory::createMatch(const std::string& cnt,
                                                   CPGMatchProfile& profile)
{
    try {
        auto match = nlohmann::json::parse(cnt);
        if (match.is_null())
        {
            LOG_ERROR << "json is invalid!!!!!";
            return {};
        }
        
        int startTime = (int)BetCoreAlgo::GetDateTimeIntFromFormat(match["startTime"].get<std::string>().c_str());
        profile.startTime = startTime;
        profile.matchName = match["matchName"].get<std::string>();
        profile.startCondition = match["startCondition"];
        profile.minPlayersAmount = match["minPlayersAmount"];
        profile.maxPlayersAmount = match["maxPlayersAmount"];
        profile.firstBlindLevel = match["firstBlindLevel"];
        profile.entryfee = match["entryfee"];
        profile.operatorTime = match["operatorTime"];
        profile.initChipsAmount = match["initChipsAmount"];
        profile.increaseBlindTime = match["increaseBlindTime"];
        profile.rewardType = match["rewardType"];
        profile.bonusID = match["bonusID"];
        profile.guaranteePerson = match["guaranteePerson"];
        profile.matchType = match["matchType"];
        profile.stopCondition = match["stopCondition"];
        profile.delayJoinLevel = match["delayJoinLevel"];
        profile.finishCondition = match["finishCondition"];
        profile.finishParam = match["finishParam"];
        return true;
        
    } catch (...) {
        LOG_ERROR << "json is invalid";
    }
    
    return false;
}














