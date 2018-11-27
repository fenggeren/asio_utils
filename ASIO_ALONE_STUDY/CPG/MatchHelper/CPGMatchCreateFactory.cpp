//
//  CPGMatchCreateFactory.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CPGMatchCreateFactory.hpp"
#include "../Third/json/json.hpp"
#include "../../Net/FASIO.hpp"
#include "BetAlgorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "CPGMatchStructHelper.hpp"


using namespace fasio::logging;

#define INVALID_MID -1

CPGMatchCreateFactory::CPGMatchCreateFactory()
:connector_()
{
    if (!connector_.connectDB("CPGLive",
                         "root", "123456789", "127.0.0.1"))
    {
        LOG_ERROR << " connect CPGLive error";
    }
}

std::map<int, std::shared_ptr<CPGMatchProfile>>
CPGMatchCreateFactory::initialize()
{
    auto oldmatches = loadFromDB();
    auto newmatches = loadFromFile();
    
    std::map<int, std::shared_ptr<CPGMatchProfile>> matches;
    
    for(auto& match : oldmatches)
    {
        matches[match->mid] = match;
    }
    
    for(auto& match : newmatches)
    {
        matches[match->mid] = match;
    }
    return matches;
}

std::list<std::shared_ptr<CPGMatchProfile>>
CPGMatchCreateFactory::loadFromDB()
{
    time_t cur = time(NULL);
    std::stringstream cmd;
    cmd << "SELECT * FROM match_infos WHERE "
    << "start_time > " << cur
    << " AND "
    << " match_state < " << 6
    << " ;";
    
    if (connector_.execQuery(cmd.str()))
    {
        return {};
    }
    
    std::list<std::shared_ptr<CPGMatchProfile>> profiles;
    for(int i = 0; i < connector_.queryRowNum(); i++)
    {
        auto& maps = connector_.fieldsValue();
        std::shared_ptr<CPGMatchProfile> profile(new CPGMatchProfile);
        mapConvertStruct(maps, *profile.get());
        connector_.queryNext();
        profiles.push_back(profile);
    }
    
    return profiles;
}

// json 创建
std::shared_ptr<CPGMatchProfile> CPGMatchCreateFactory::createMatch(const std::string& cnt)
{
    std::shared_ptr<CPGMatchProfile> profile = nullptr;
    try {
        auto match = nlohmann::json::parse(cnt);
        if (match.is_null())
        {
            LOG_ERROR << "json is invalid!!!!!";
            return {};
        }
        return createMatch(match);

        
    } catch (...) {
        LOG_ERROR << "json is invalid";
    }
    
    return profile;
}

std::shared_ptr<CPGMatchProfile>
CPGMatchCreateFactory::createMatch(std::shared_ptr<CPGMatchProfile> profile)
{
    auto pair = checkValidAndStore(profile);
    if (pair.first) // false 已存在
    {
        profile->mid = pair.second;
        LOG_MINFO << "new match mid: " << pair.second;
        return profile;
    }
    else
    {
        return nullptr;
    }
}

// 可能已有一些其他字段的校验--!
std::pair<bool, int>
CPGMatchCreateFactory::checkValidAndStore(std::shared_ptr<CPGMatchProfile> profile)
{
    bool res = false;
    int mid = queryMid(profile);
    
    // 查找比赛是否已经创建。
    if (mid == INVALID_MID)
    {
        res = true;
        mid = CPGMatchCreateFactory::createAndStoreMatch(*profile.get(), connector_);
    }
    
    
    if (mid == INVALID_MID)
    {
        mid = queryMid(profile);
    }

    
    return {res, mid};
}

int CPGMatchCreateFactory::queryMid(std::shared_ptr<CPGMatchProfile> profile)
{
    return CPGMatchCreateFactory::queryMatchMid(*profile.get(), connector_);
}

std::shared_ptr<CPGMatchProfile>
CPGMatchCreateFactory::createMatch(const nlohmann::json& match)
{
    std::shared_ptr<CPGMatchProfile> profile = nullptr;
    try {
        if (match.is_null())
        {
            LOG_ERROR << "json is invalid!!!!!";
            return profile;
        } 
        profile = std::make_shared<CPGMatchProfile>();
        int startTime = (int)BetCoreAlgo::GetDateTimeIntFromFormat(match["start_time"].get<std::string>().c_str());
        profile->start_time = startTime;
        profile->name = match["name"].get<std::string>();
        profile->start_condition = match["start_condition"];
        profile->min_players_amount = match["min_players_amount"];
        profile->max_players_amount = match["max_players_amount"];
        profile->first_blind_level = match["first_blind_level"];
        profile->entryfee = match["entryfee"];
        profile->operator_time = match["operator_time"];
        profile->init_chips_amount = match["init_chips_amount"];
        profile->increase_blind_time = match["increase_blind_time"];
        profile->reward_type = match["reward_type"];
        profile->entry_pool = match["entry_pool"];
        profile->reward_coin = match["reward_coin"];
        profile->bonusid = match["bonusid"];
        profile->guarantee_person = match["guarantee_person"];
        profile->match_type = match["match_type"];
        profile->stop_condition = match["stop_condition"];
        profile->delay_join_level = match["delay_join_level"];
        profile->finish_condition = match["finish_condition"];
        profile->finish_param = match["finish_param"];
        profile->current_blind_level = profile->first_blind_level;
     
        if (profile->entry_pool == 0)
        {
            profile->entry_pool = profile->entryfee;
        }
        
        auto pair = checkValidAndStore(profile);
        if (pair.first) // false 已存在
        {
            profile->mid = pair.second;
            LOG_MINFO << "new match mid: " << pair.second;
            return profile;
        }
        else
        {
            return nullptr;
        }
    } catch (...) {
        LOG_ERROR << "json is invalid";
    }
    
    return profile;
}

int
CPGMatchCreateFactory::createAndStoreMatch(CPGMatchProfile& profile,
                               SQLConnector& connector)
{
    // 存储
    std::stringstream save;
    
    auto paramap = structConvertMap(profile);
    std::stringstream fss;
    std::stringstream vss;
    
    auto index = paramap.size();
    for(auto& pair : paramap)
    {
        index--;
        fss << pair.first;
        vss << "'" << pair.second <<"'";
        if (index > 0)
        {
            fss << ",";
            vss << ",";
        }
    }
    save << "INSERT INTO match_infos ( "
    << fss.str() << " ) "
    << " VALUES ( "
    << vss.str() << " ); ";
    
    if (connector.execQuery(save.str()))
    {
        LOG_ERROR << "save error! " << connector.errorStr();
    }
    return queryMatchMid(profile, connector);
}

std::pair<bool, int>
CPGMatchCreateFactory::checkValidMatch(CPGMatchProfile& profile,
                SQLConnector& connector)
{
    bool res = false;
    int mid = queryMatchMid(profile, connector);
    
    // 查找比赛是否已经创建。
    if (mid == INVALID_MID)
    {
        res = true;
    }
    return {res, mid};
}

int
CPGMatchCreateFactory::queryMatchMid(CPGMatchProfile& profile,
                         SQLConnector& connector)
{
    int mid = INVALID_MID;
    
    std::stringstream ss;
    ss << "SELECT mid FROM match_infos WHERE ";
    ss << " start_time=" << profile.start_time;
    ss << " AND match_type=" << profile.match_type;
    ss << " limit 1;";
    if (connector.execQuery(ss.str()))
    {
        return mid;
    }
    
    
    if (connector.queryRowNum() == 1)
    {
        connector.fieldData("mid", mid);
    }
    return mid;
}


std::list<std::shared_ptr<CPGMatchProfile>>
CPGMatchCreateFactory::loadFromFile()
{
    using nlohmann::json;
    std::ifstream in("MatchConfig.json");
    nlohmann::json file_json;
    in >> file_json;
    
    if (file_json.is_null())
    {
        LOG_MINFO << "match.config.json file is invalid!!!!!";
        return {};
    }
    
    std::list<std::shared_ptr<CPGMatchProfile>> matches;
    std::vector<json> matches_json = file_json["matches"];
    for(auto& match : matches_json)
    {
        auto match_info = createMatch(match);
        if (match_info)
        {
            matches.push_back(match_info);
        }
    }
    return matches;
}










