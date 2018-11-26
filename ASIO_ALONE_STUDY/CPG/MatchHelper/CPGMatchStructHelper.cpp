//
//  CPGMatchStructHelper.cpp
//  WebServer
//
//  Created by huanao on 2018/11/23.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CPGMatchStructHelper.hpp"
#include "../BetAlgorithm.h"

std::map<std::string, std::string>
structConvertMap(const CPGMatchProfile& match)
{
    return
    {
        {"mid", std::to_string(match.mid)},
        {"start_time", std::to_string(match.start_time)},
        {"total_player_amount", std::to_string(match.total_player_amount)},
        {"remained_player_amount", std::to_string(match.remained_player_amount)},
        {"match_state", std::to_string(match.match_state)},
        {"current_blind_level",std::to_string(match.current_blind_level)},
        {"entryfee", std::to_string(match.entryfee)},
        {"init_chips_amount", std::to_string(match.init_chips_amount)},
        {"min_players_amount", std::to_string(match.min_players_amount)},
        {"max_players_amount", std::to_string(match.max_players_amount)},
        {"operator_time", std::to_string(match.operator_time)},
        {"increase_blind_time", std::to_string(match.increase_blind_time)},
        {"first_blind_level", std::to_string(match.first_blind_level)},
        {"delay_join_level", std::to_string(match.delay_join_level)},
        {"stop_condition", std::to_string(match.stop_condition)},
        {"start_condition", std::to_string(match.start_condition)},
        {"reward_type", std::to_string(match.reward_type)},
        {"match_type", std::to_string(match.match_type)},
        {"bonusid", std::to_string(match.bonusid)},
        {"guarantee_person", std::to_string(match.guarantee_person)},
        {"name", match.name},
        {"enter_code", std::to_string(match.enter_code)},
        {"entry_pool", std::to_string(match.entry_pool)},
        {"reward_coin", std::to_string(match.reward_coin)},
        {"finish_condition", std::to_string(match.finish_condition)},
        {"finish_param", std::to_string(match.finish_param)},
        {"match_auto_cancel_second", std::to_string(match.match_auto_cancel_second)},
        {"bankid", std::to_string(match.bankid)},
        {"end_time", std::to_string(match.end_time)}
    };
}

void mapConvertStruct(std::map<std::string, std::string>& dict,
                      CPGMatchProfile& match)
{
    match.mid = std::atoi(dict["mid"].c_str());
    match.start_time = std::atoi(dict["begin_date_time"].c_str());
    match.total_player_amount = std::atoi(dict["total_joined_player"].c_str());
    match.remained_player_amount = std::atoi(dict["remained_player"].c_str());
    match.match_state = std::atoi(dict["match_state"].c_str());
    match.current_blind_level = std::atoi(dict["current_blind_level"].c_str());
    match.entryfee = std::atoi(dict["entryfee_coin"].c_str());
    match.init_chips_amount = std::atoi(dict["init_chips_amount"].c_str());
    match.min_players_amount = std::atoi(dict["min_players_amount"].c_str());
    match.max_players_amount = std::atoi(dict["max_players_amount"].c_str());
    match.operator_time = std::atoi(dict["speed"].c_str());
    match.increase_blind_time = std::atoi(dict["increase_blind_time"].c_str());
    match.first_blind_level = std::atoi(dict["start_blind_level"].c_str());
    match.delay_join_level = std::atoi(dict["delay_join_level"].c_str());
    match.start_condition = std::atoi(dict["start_condition"].c_str());
    match.reward_type = std::atoi(dict["reward_type"].c_str());
    match.match_type = std::atoi(dict["match_type"].c_str());
    match.bonusid = std::atoi(dict["bonus_id"].c_str());
    match.guarantee_person = std::atoi(dict["guarantee_person"].c_str());
    match.enter_code = std::atoi(dict["enter_code"].c_str());
    match.entry_pool = std::atoi(dict["entry_pool"].c_str());
    match.reward_coin = std::atoi(dict["reward_coin"].c_str());
    match.finish_condition = std::atoi(dict["finish_condition"].c_str());
    match.finish_param = std::atoi(dict["finish_param"].c_str());
    match.match_auto_cancel_second = std::atoi(dict["match_auto_cancel_second"].c_str());
    match.bankid = std::atoi(dict["bankid"].c_str());
    match.stop_condition = std::atoi(dict["stop_condition"].c_str());
    match.name = dict["name"];
    match.end_time = std::atoi(dict["end_time"].c_str());
}


namespace nlohmann
{
    using nlohmann::json;
    
    void to_json(json& j, const CPGMatchProfile& match)
    {
        j = json{
            {"mid", match.mid},
            {"start_time", match.start_time},
            {"total_player_amount",match.total_player_amount},
            {"remained_player_amount",match.remained_player_amount},
            {"match_state", match.match_state},
            {"current_blind_level",match.current_blind_level},
            {"entryfee", match.entryfee},
            {"init_chips_amount",match.init_chips_amount},
            {"min_players_amount",match.min_players_amount},
            {"max_players_amount",match.max_players_amount},
            {"operator_time", match.operator_time},
            {"increase_blind_time",match.increase_blind_time},
            {"first_blind_level",match.first_blind_level},
            {"delay_join_level", match.delay_join_level},
            {"stop_condition", match.stop_condition},
            {"start_condition", match.start_condition},
            {"reward_type", match.reward_type},
            {"match_type", match.match_type},
            {"bonusid", match.bonusid},
            {"guarantee_person", match.guarantee_person},
            {"name", match.name},
            {"enter_code", match.enter_code},
            {"entry_pool", match.entry_pool},
            {"reward_coin", match.reward_coin},
            {"finish_condition", match.finish_condition},
            {"finish_param", match.finish_param},
            {"match_auto_cancel_second",match.match_auto_cancel_second},
            {"bankid", match.bankid},
            {"end_time", match.end_time}
        };
    }
    void from_json(const json& j, CPGMatchProfile& match)
    {
        match.name = j["name"].get<std::string>();
        match.start_condition = j["start_condition"];
        match.min_players_amount = j["min_players_amount"];
        match.max_players_amount = j["max_players_amount"];
        match.first_blind_level = j["first_blind_level"];
        match.entryfee = j["entryfee"];
        match.operator_time = j["operator_time"];
        match.init_chips_amount = j["init_chips_amount"];
        match.increase_blind_time = j["increase_blind_time"];
        match.reward_type = j["reward_type"];
        match.entry_pool = j["entry_pool"];
        match.reward_coin = j["reward_coin"];
        match.bonusid = j["bonusid"];
        match.guarantee_person = j["guarantee_person"];
        match.match_type = j["match_type"];
        match.stop_condition = j["stop_condition"];
        match.delay_join_level = j["delay_join_level"];
        match.finish_condition = j["finish_condition"];
        match.finish_param = j["finish_param"];
        if (!j["mid"].is_null())
        {
            match.mid = j["mid"];
        }
        if (!j["end_time"].is_null())
        {
            match.end_time = j["end_time"];
        }
        if (!j["match_auto_cancel_second"].is_null())
        {
            match.match_auto_cancel_second = j["match_auto_cancel_second"];
        }
        if (!j["bankid"].is_null())
        {
            match.bankid = j["bankid"];
        }
        if (!j["current_blind_level"].is_null())
        {
            match.current_blind_level = j["current_blind_level"];
        }
        if (!j["total_player_amount"].is_null())
        {
            match.total_player_amount = j["total_player_amount"];
        }
        if (!j["remained_player_amount"].is_null())
        {
            match.remained_player_amount = j["remained_player_amount"];
        }
        if (!j["match_state"].is_null())
        {
            match.match_state = j["match_state"];
        }
        if (!j["enter_code"].is_null())
        {
            match.enter_code = j["enter_code"];
        }
        if (j["start_time"].is_number())
        {
            match.start_time = j["start_time"];
        }
        else if (!j["start_time"].is_string())
        {
            match.start_time =  (int)BetCoreAlgo::GetDateTimeIntFromFormat(j["start_time"].get<std::string>().c_str());
        }
    }
}
