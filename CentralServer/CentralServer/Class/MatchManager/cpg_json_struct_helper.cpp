//
//  cpg_json_struct_helper.cpp
//  reids_demo
//
//  Created by HuanaoGroup on 2018/3/27.
//  Copyright © 2018年 HuanaoGroup. All rights reserved.
//

#include "cpg_json_struct_helper.h"
#include <iostream>


namespace nlohmann {

    void to_json(json& j,
                 const cpg_match_generate_date& generate_date)
    {
        j = json{{"date_duration", generate_date.date_duration},
            {"weeks", generate_date.weeks},
            {"times", generate_date.times},
            {"generate_date", generate_date.generate_date},
            {"generate_time_cycle", generate_date.generate_time_cycle},
            {"type", generate_date.type}, 
            {"rule_id", generate_date.rule_id}
        };
    }
    
    void from_json(const json& j, cpg_match_generate_date& generate_date)
    {
        generate_date.date_duration = j["date_duration"].get<std::string>();
        generate_date.weeks = j["weeks"].get<std::string>();
        generate_date.times = j["times"].get<std::string>();
        generate_date.generate_date = j["generate_date"].get<std::string>();
        generate_date.generate_time_cycle = j["generate_time_cycle"].get<int>();
        generate_date.type = (cpg_match_create_type)j["type"].get<int>();
        generate_date.rule_id = j["rule_id"].get<int>();
    }
    
    void to_json(json& j,
                 const cpg_match_rule& rule)
    {
        j = json{
            {"entryfee_coin", rule.entryfee_coin},
            {"init_chips_amount", rule.init_chips_amount},
            {"rule_id", rule.rule_id},
            {"min_players_amount", rule.min_players_amount},
            {"max_players_amount", rule.max_players_amount},
            {"speed", rule.speed},
            {"increase_blind_time", rule.increase_blind_time},
            {"start_blind_level", rule.start_blind_level},
            {"end_match_type",rule.end_match_type},
            {"end_match_param", rule.end_match_param},
            {"delay_join", rule.delay_join},
            {"start_condition", rule.start_condition},
            {"reward_type", rule.reward_type},
            {"match_type", rule.match_type},
            {"bonus_id", rule.bonus_id},
            {"guarantee_person", rule.guarantee_person},
            {"match_name", rule.match_name},
            {"entry_condition", rule.entry_condition},
            {"extra", rule.extra}
        };
    }
    
    void from_json(const json& j, cpg_match_rule& rule)
    {
        try {
            if (!j.at("rule_id").is_null())
            {
                rule.rule_id = j["rule_id"].get<int>();
            }
        } catch (...)
        {
            std::cout << "from_json: parse  rule_id wrong" << std::endl;
        }

        rule.entryfee_coin = j["entryfee_coin"].get<int>();
        rule.init_chips_amount = j["init_chips_amount"].get<int>();
        rule.min_players_amount = j["min_players_amount"].get<int>();
        rule.max_players_amount = j["max_players_amount"].get<int>();
        rule.speed = j["speed"].get<int>();
        rule.increase_blind_time = j["increase_blind_time"].get<int>();
        rule.start_blind_level = j["start_blind_level"].get<int>();
        rule.end_match_type = j["end_match_type"].get<int>();
        rule.end_match_param = j["end_match_param"].get<int>();
        rule.delay_join = j["delay_join"].get<int>();
        rule.start_condition = (matchStartConditionType)j["start_condition"].get<int>();
        rule.reward_type = (matchRewardType)j["reward_type"].get<int>();
        rule.match_type = (ENUM_ROOM_TYPE)j["match_type"].get<int>();
        rule.bonus_id = j["bonus_id"].get<int>();
        rule.guarantee_person = j["guarantee_person"].get<int>();
        rule.match_name = j["match_name"].get<std::string>();
        rule.entry_condition = j["entry_condition"].get<std::string>();
        rule.extra = j["extra"].get<std::string>();
    }
    
    void to_json(json& j,
                 const cpg_match_detail& detail)
    {
        j = json{
            {"match_id", detail.match_id},
            {"begin_date_time", detail.begin_date_time},
            {"rule_id", detail.rule_id},
            {"total_joined_player", detail.total_joined_player},
            {"remained_player", detail.remained_player},
            {"match_state", detail.match_state},
            {"current_blind_level", detail.current_blind_level},
        };
    }
    
    void from_json(const json& j, cpg_match_detail& detail)
    {
        detail.match_id = j["match_id"].get<int>();
        detail.begin_date_time = j["begin_date_time"].get<int>();
        detail.rule_id = j["rule_id"].get<int>();
        detail.total_joined_player = j["total_joined_player"].get<int>();
        detail.remained_player = j["remained_player"].get<int>();
        detail.match_state = (ENUM_MTT_MATCH_STATE)j["match_state"].get<int>();
        detail.current_blind_level = j["current_blind_level"].get<int>();
    }
    
    void to_json(json& j,
                 const cpg_match_multiday::cpg_match_item& multi_item)
    {
        j = json{
            {"match_id", multi_item.match_id},
            {"round", multi_item.round},
            {"end_type", multi_item.end_type},
            {"end_value", multi_item.end_value},
        };
    }
    void from_json(const json& j, cpg_match_multiday::cpg_match_item& multi_item)
    {
        multi_item.match_id = j["match_id"].get<int>();
        multi_item.round = j["round"].get<int>();
        multi_item.end_type = j["end_type"].get<int>();
        multi_item.end_value = j["end_value"].get<int>();
    }
    
    
    void to_json(json& j,
                 const cpg_match_multiday& multiday)
    {
        std::list<json> items;
        for (auto& item : multiday.items)
        {
            items.emplace_back(item);
        }
        j = json{
            {"multiday_id", multiday.multiday_id},
            {"items", items},
        };
    }
    
    void from_json(const json& j, cpg_match_multiday& multiday)
    {
        multiday.multiday_id = j["multiday_id"].get<int>();
        auto jsons = j["items"].array();
        std::list<cpg_match_multiday::cpg_match_item> items;
        for (auto json : jsons)
        {
            items.emplace_back(json);
        }
        multiday.items = items;
    }
    
    
    void to_json(json& j,
                 const cpg_match_join_info& join_info)
    {
        j = json{
            {"user_id", join_info.user_id},
            {"proxy_id", join_info.proxy_id},
            {"cur_state", (int)join_info.cur_state},
            {"join_time", join_info.join_time},
            {"change_time", join_info.change_time},
        };
    }
    void from_json(const json& j, cpg_match_join_info& join_info)
    {
        join_info.user_id = j["user_id"].get<int>();
        join_info.proxy_id = j["proxy_id"].get<int>();
        join_info.cur_state = (cpg_match_join_info::state)j["cur_state"].get<int>();
        join_info.join_time = j["join_time"].get<int>();
        join_info.change_time = j["change_time"].get<int>();
    }

    void to_json(json& j,
                 const cpg_match_info& match)
    {
        j = json{
            {"match_id", match.match_id},
            {"begin_date_time", match.begin_date_time},
            {"rule_id", match.rule_id},
            {"total_joined_player", match.total_joined_player},
            {"remained_player", match.remained_player},
            {"match_state", match.match_state},
            {"current_blind_level",match.current_blind_level},
            {"entryfee_coin", match.entryfee_coin},
            {"init_chips_amount", match.init_chips_amount},
            {"rule_id", match.rule_id},
            {"min_players_amount", match.min_players_amount},
            {"max_players_amount", match.max_players_amount},
            {"speed", match.speed},
            {"increase_blind_time", match.increase_blind_time},
            {"start_blind_level", match.start_blind_level},
            {"end_match_type", match.end_match_type},
            {"end_match_param", match.end_match_param},
            {"delay_join", match.delay_join},
            {"start_condition", match.start_condition},
            {"reward_type", match.reward_type},
            {"match_type", match.match_type},
            {"bonus_id", match.bonus_id},
            {"guarantee_person", match.guarantee_person},
            {"match_name", match.match_name},
            {"entry_condition", match.entry_condition},
            {"extra", match.extra}
        };
    }
    void from_json(const json& j, cpg_match_info& match)
    {
        match.match_id = j["match_id"].get<int>();
        match.begin_date_time = j["begin_date_time"].get<int>();
        match.rule_id = j["rule_id"].get<int>();
        match.total_joined_player = j["total_joined_player"].get<int>();
        match.remained_player = j["remained_player"].get<int>();
        match.match_state = (ENUM_MTT_MATCH_STATE)j["match_state"].get<int>();
        match.current_blind_level = j["current_blind_level"].get<int>();
        match.entryfee_coin = j["entryfee_coin"].get<int>();
        match.init_chips_amount = j["init_chips_amount"].get<int>();
        match.rule_id = j["rule_id"].get<int>();
        match.min_players_amount = j["min_players_amount"].get<int>();
        match.max_players_amount = j["max_players_amount"].get<int>();
        match.speed = j["speed"].get<int>();
        match.increase_blind_time = j["increase_blind_time"].get<int>();
        match.start_blind_level = j["start_blind_level"].get<int>();
        match.end_match_type = j["end_match_type"].get<int>();
        match.end_match_param = j["end_match_param"].get<int>();
        match.delay_join = j["delay_join"].get<int>();
        match.start_condition = (matchStartConditionType)j["start_condition"].get<int>();
        match.reward_type = (matchRewardType)j["reward_type"].get<int>();
        match.match_type = (ENUM_ROOM_TYPE)j["match_type"].get<int>();
        match.bonus_id = j["bonus_id"].get<int>();
        match.guarantee_person = j["guarantee_person"].get<int>();
        match.match_name = j["match_name"];
        match.entry_condition = j["entry_condition"];
        match.extra = j["extra"];
    }

}



namespace cpg_match_parser
{
  
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_detail& detail)
    {
        return {
            {"match_id", std::to_string(detail.match_id)},
            {"begin_date_time", std::to_string(detail.begin_date_time)},
            {"rule_id", std::to_string(detail.rule_id)},
            {"total_joined_player", std::to_string(detail.total_joined_player)},
            {"remained_player", std::to_string(detail.remained_player)},
            {"match_state", std::to_string(detail.match_state)},
            {"current_blind_level", std::to_string(detail.current_blind_level)},
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_detail& detail)
    {
        detail.match_id = std::atoi(dict["match_id"].c_str());
        detail.begin_date_time = std::atoi(dict["begin_date_time"].c_str());
        detail.rule_id = std::atoi(dict["rule_id"].c_str());
        detail.total_joined_player = std::atoi(dict["total_joined_player"].c_str());
        detail.remained_player = std::atoi(dict["remained_player"].c_str());
        detail.match_state = (ENUM_MTT_MATCH_STATE)std::atoi(dict["match_state"].c_str());
        detail.current_blind_level = std::atoi(dict["current_blind_level"].c_str());
    }
    
        std::map<std::string, std::string>
    struct_convert_map(const cpg_match_info& match)
    {
        return {
            {"match_id", std::to_string(match.match_id)},
            {"begin_date_time", std::to_string(match.begin_date_time)},
            {"rule_id", std::to_string(match.rule_id)},
            {"total_joined_player", std::to_string(match.total_joined_player)},
            {"remained_player", std::to_string(match.remained_player)},
            {"match_state", std::to_string(match.match_state)},
            {"current_blind_level",std::to_string(match.current_blind_level)},
            {"entryfee_coin", std::to_string(match.entryfee_coin)},
            {"init_chips_amount", std::to_string(match.init_chips_amount)},
            {"rule_id", std::to_string(match.rule_id)},
            {"min_players_amount", std::to_string(match.min_players_amount)},
            {"max_players_amount", std::to_string(match.max_players_amount)},
            {"speed", std::to_string(match.speed)},
            {"increase_blind_time", std::to_string(match.increase_blind_time)},
            {"start_blind_level", std::to_string(match.start_blind_level)},
            {"end_match_type", std::to_string(match.end_match_type)},
            {"end_match_param", std::to_string(match.end_match_param)},
            {"delay_join", std::to_string(match.delay_join)},
            {"start_condition", std::to_string(match.start_condition)},
            {"reward_type", std::to_string(match.reward_type)},
            {"match_type", std::to_string(match.match_type)},
            {"bonus_id", std::to_string(match.bonus_id)},
            {"guarantee_person", std::to_string(match.guarantee_person)},
            {"match_name", match.match_name},
            {"entry_condition", match.entry_condition},
            {"extra", match.extra}
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_info& match)
    {
        match.match_id = std::atoi(dict["match_id"].c_str());
        match.begin_date_time = std::atoi(dict["begin_date_time"].c_str());
        match.rule_id = std::atoi(dict["rule_id"].c_str());
        match.total_joined_player = std::atoi(dict["total_joined_player"].c_str());
        match.remained_player = std::atoi(dict["remained_player"].c_str());
        match.match_state = (ENUM_MTT_MATCH_STATE)std::atoi(dict["match_state"].c_str());
        match.current_blind_level = std::atoi(dict["current_blind_level"].c_str());
        match.entryfee_coin = std::atoi(dict["entryfee_coin"].c_str());
        match.init_chips_amount = std::atoi(dict["init_chips_amount"].c_str());
        match.rule_id = std::atoi(dict["rule_id"].c_str());
        match.min_players_amount = std::atoi(dict["min_players_amount"].c_str());
        match.max_players_amount = std::atoi(dict["max_players_amount"].c_str());
        match.speed = std::atoi(dict["speed"].c_str());
        match.increase_blind_time = std::atoi(dict["increase_blind_time"].c_str());
        match.start_blind_level = std::atoi(dict["start_blind_level"].c_str());
        match.end_match_type = std::atoi(dict["end_match_type"].c_str());
        match.end_match_param = std::atoi(dict["end_match_param"].c_str());
        match.delay_join = std::atoi(dict["delay_join"].c_str());
        match.start_condition = (matchStartConditionType)std::atoi(dict["start_condition"].c_str());
        match.reward_type = (matchRewardType)std::atoi(dict["reward_type"].c_str());
        match.match_type = (ENUM_ROOM_TYPE)std::atoi(dict["match_type"].c_str());
        match.bonus_id = std::atoi(dict["bonus_id"].c_str());
        match.guarantee_person = std::atoi(dict["guarantee_person"].c_str());
        match.match_name = dict["match_name"];
        match.entry_condition = dict["entry_condition"];
        match.extra = dict["extra"];
    }


    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_rule& rule)
    {
        return {
            {"entryfee_coin", std::to_string(rule.entryfee_coin)},
            {"init_chips_amount", std::to_string(rule.init_chips_amount)},
            {"rule_id", std::to_string(rule.rule_id)},
            {"min_players_amount", std::to_string(rule.min_players_amount)},
            {"max_players_amount", std::to_string(rule.max_players_amount)},
            {"speed", std::to_string(rule.speed)},
            {"increase_blind_time", std::to_string(rule.increase_blind_time)},
            {"start_blind_level", std::to_string(rule.start_blind_level)},
            {"end_match_type", std::to_string(rule.end_match_type)},
            {"end_match_param", std::to_string(rule.end_match_param)},
            {"delay_join", std::to_string(rule.delay_join)},
            {"start_condition", std::to_string(rule.start_condition)},
            {"reward_type", std::to_string(rule.reward_type)},
            {"match_type", std::to_string(rule.match_type)},
            {"bonus_id", std::to_string(rule.bonus_id)},
            {"guarantee_person", std::to_string(rule.guarantee_person)},
            {"match_name", rule.match_name},
            {"entry_condition", rule.entry_condition},
            {"extra", rule.extra}
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict, 
                            cpg_match_rule& rule)
    {
        rule.entryfee_coin = std::atoi(dict["entryfee_coin"].c_str());
        rule.init_chips_amount = std::atoi(dict["init_chips_amount"].c_str());
        rule.rule_id = std::atoi(dict["rule_id"].c_str());
        rule.min_players_amount = std::atoi(dict["min_players_amount"].c_str());
        rule.max_players_amount = std::atoi(dict["max_players_amount"].c_str());
        rule.speed = std::atoi(dict["speed"].c_str());
        rule.increase_blind_time = std::atoi(dict["increase_blind_time"].c_str());
        rule.start_blind_level = std::atoi(dict["start_blind_level"].c_str());
        rule.end_match_type = std::atoi(dict["end_match_type"].c_str());
        rule.end_match_param = std::atoi(dict["end_match_param"].c_str());
        rule.delay_join = std::atoi(dict["delay_join"].c_str());
        rule.start_condition = (matchStartConditionType)std::atoi(dict["start_condition"].c_str());
        rule.reward_type = (matchRewardType)std::atoi(dict["reward_type"].c_str());
        rule.match_type = (ENUM_ROOM_TYPE)std::atoi(dict["match_type"].c_str());
        rule.bonus_id = std::atoi(dict["bonus_id"].c_str());
        rule.guarantee_person = std::atoi(dict["guarantee_person"].c_str());
        rule.match_name = dict["match_name"];
        rule.entry_condition = dict["entry_condition"];
        rule.extra = dict["extra"];
    }
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_player_account& player)
    {
        return {
            {"user_id", std::to_string(player.user_id)},
            {"player_heart_time", std::to_string(player.player_heart_time)},
            {"player_client_version", std::to_string(player.player_client_version)},
            {"player_name", player.player_name}, 
            {"token", player.token}
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_player_account& account)
    {
        account.user_id = std::atoi(dict["user_id"].c_str());
        account.player_heart_time = std::atoi(dict["player_heart_time"].c_str());
        account.player_client_version = std::atoi(dict["player_client_version"].c_str());
        account.player_name = dict["player_name"];
        account.token = dict["token"];
    }
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_player& player)
    {
        return {
            {"user_id", std::to_string(player.user_id)},
            {"match_id", std::to_string(player.match_id)},
            {"chips_amount", std::to_string(player.chips_amount)},
            // {"table_id", std::to_string(player.table_id)},
//            {"last_chips_amount", player.last_chips_amount},
//            {"ranking", player.ranking},
            {"user_name", player.user_name},
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_player& player)
    {
        player.user_id = std::atoi(dict["user_id"].c_str());
        player.match_id = std::atoi(dict["match_id"].c_str());
        player.chips_amount = std::atoi(dict["chips_amount"].c_str());
        // player.table_id = std::atoi(dict["table_id"].c_str());
        player.user_name = dict["user_name"];
    }
     
    
    std::vector<std::string>
    get_params_vector(const cpg_player_account& player)
    {
        return {"user_id",
            "player_name",
            "player_heart_time",
            "player_client_version"};
    }
    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_player_reward& reward)
    {
        return {
            {"user_id", std::to_string(reward.user_id)},
            {"rank", std::to_string(reward.rank)},
            {"type", std::to_string(reward.type)},
            {"value", std::to_string(reward.value)},
            {"user_name", reward.user_name},
            {"name", reward.name},
            // {"description", reward.description},
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_player_reward& reward)
    {
        reward.user_id = std::atoi(dict["user_id"].c_str());
        reward.rank = std::atoi(dict["rank"].c_str());
        reward.type = (CPGBounsType)std::atoi(dict["type"].c_str());
        reward.value = std::atoi(dict["value"].c_str());
        reward.user_name = dict["user_name"];
        reward.name = dict["name"];
        // reward.description = dict["description"];
    }
    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_join_info& join)
    {
        return {
            {"user_id", std::to_string(join.user_id)},
            {"proxy_id", std::to_string(join.proxy_id)},
            {"cur_state", std::to_string(join.cur_state)},
            {"join_time", std::to_string(join.join_time)},
            {"change_time", std::to_string(join.change_time)},
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_join_info& join)
    {
        join.user_id = std::atoi(dict["user_id"].c_str());
        join.proxy_id = std::atoi(dict["proxy_id"].c_str());
        join.cur_state =  (cpg_match_join_info::state)
                            std::atoi(dict["cur_state"].c_str());
        join.join_time = std::atoi(dict["join_time"].c_str());
        join.change_time = std::atoi(dict["change_time"].c_str());
    }
    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_multiday::cpg_match_item& multi_item)
    {
        return {
            {"match_id", std::to_string(multi_item.match_id)},
            {"round", std::to_string(multi_item.round)},
            {"end_type", std::to_string(multi_item.end_type)},
            {"end_value", std::to_string(multi_item.end_value)},
        };
    }
    
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_multiday::cpg_match_item& multi_item)
    {
        multi_item.match_id = std::atoi(dict["match_id"].c_str());
        multi_item.round = std::atoi(dict["round"].c_str());
        multi_item.end_type =  std::atoi(dict["end_type"].c_str());
        multi_item.end_value =  std::atoi(dict["end_value"].c_str());
    }
    
}






namespace struct_utils
{
    void match_rule_to_info(const cpg_match_rule& rule, cpg_match_info& info)
    {
        info.rule_id = rule.rule_id;
        info.entryfee_coin = rule.entryfee_coin;
        info.init_chips_amount = rule.init_chips_amount;
        info.min_players_amount = rule.min_players_amount;
        info.max_players_amount = rule.max_players_amount;
        info.speed = rule.speed;
        info.increase_blind_time = rule.increase_blind_time;
        info.start_blind_level = rule.start_blind_level;
        info.end_match_type = rule.end_match_type;
        info.end_match_param = rule.end_match_param;
        info.delay_join = rule.delay_join;
        info.start_condition = rule.start_condition;
        info.reward_type = rule.reward_type;
        info.match_type = rule.match_type;
        info.bonus_id = rule.bonus_id;
        info.guarantee_person = rule.guarantee_person;
        info.match_name = rule.match_name;
        info.entry_condition = rule.entry_condition;
        info.extra = rule.extra;
    }

    void match_rule_to_info(cpg_match_rule&& rule, cpg_match_info& info)
    {
        info.rule_id = rule.rule_id;
        info.entryfee_coin = rule.entryfee_coin;
        info.init_chips_amount = rule.init_chips_amount;
        info.min_players_amount = rule.min_players_amount;
        info.max_players_amount = rule.max_players_amount;
        info.speed = rule.speed;
        info.increase_blind_time = rule.increase_blind_time;
        info.start_blind_level = rule.start_blind_level;
        info.end_match_type = rule.end_match_type;
        info.end_match_param = rule.end_match_param;
        info.delay_join = rule.delay_join;
        info.start_condition = rule.start_condition;
        info.reward_type = rule.reward_type;
        info.match_type = rule.match_type;
        info.bonus_id = rule.bonus_id;
        info.guarantee_person = rule.guarantee_person;
        info.match_name = std::move(rule.match_name);
        info.entry_condition = std::move(rule.entry_condition);
        info.extra = std::move(rule.extra);
    }
}

bool operator<(const cpg_match_info& lv, const cpg_match_info& rv)
{
    return lv.begin_date_time < rv.begin_date_time;
}