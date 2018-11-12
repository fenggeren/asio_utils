//
//  cpg_json_struct_helper.hpp
//  reids_demo
//
//  Created by HuanaoGroup on 2018/3/27.
//  Copyright © 2018年 HuanaoGroup. All rights reserved.
//
#pragma once

#include <CPG/Third/json/json.hpp>
#include <CPG/cpg_match_struct_define.h>
#include <memory>


namespace nlohmann
{
    using nlohmann::json;
    
    void to_json(json& j,
     const cpg_match_generate_date& generate_date);
    void from_json(const json& j, cpg_match_generate_date& generate_date);
    
    void to_json(json& j,
       const cpg_match_rule& rule);
    void from_json(const json& j, cpg_match_rule& rule);
    
    void to_json(json& j,
                 const cpg_match_detail& rule);
    void from_json(const json& j, cpg_match_detail& rule);
    
    void to_json(json& j,
                 const cpg_match_multiday& multiday);
    void from_json(const json& j, cpg_match_multiday& multiday);
    
    void to_json(json& j,
                 const cpg_match_multiday::cpg_match_item& multi_item);
    void from_json(const json& j, cpg_match_multiday::cpg_match_item& multi_item);
    

    void to_json(json& j,
                 const cpg_match_join_info& join_info);
    void from_json(const json& j, cpg_match_join_info& join_info);

    void to_json(json& j,
                 const cpg_match_info& match);
    void from_json(const json& j, cpg_match_info& match);
}


//namespace struct_helper
//{
//    bool operator<(const cpg_match_detail& lv, const cpg_match_detail& rv)
//    {
//        return lv.begin_date_time < rv.begin_date_time;
//    }
//    bool operator>(const cpg_match_detail& lv, const cpg_match_detail& rv)
//    {
//        return lv.begin_date_time > rv.begin_date_time;
//    }
//    bool operator==(const cpg_match_detail& lv, const cpg_match_detail& rv)
//    {
//        return lv.begin_date_time == rv.begin_date_time &&
//                lv.match_id == rv.match_id;
//    }
//}


namespace cpg_match_parser
{
    
    template <typename T>
    std::map<std::string, std::string>
    struct_convert_map(const T& detail);
    
    template <typename T>
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            T& account);
    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_detail& detail);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_detail& detail);
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_info& match);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_info& match);
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_rule& rule);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_rule& rule);
    
    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_player_account& player);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_player_account& account);
    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_player& player);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_player& player);
    

    
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_player_reward& reward);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_player_reward& reward);
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_join_info& join);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_join_info& join);
    
    std::map<std::string, std::string>
    struct_convert_map(const cpg_match_multiday::cpg_match_item& multi_item);
    void map_convert_struct(std::map<std::string, std::string>& dict,
                            cpg_match_multiday::cpg_match_item& multi_item);
}



namespace struct_utils
{
    void match_rule_to_info(const cpg_match_rule& rule, cpg_match_info& info);
    void match_rule_to_info(cpg_match_rule&& rule, cpg_match_info& info);
}

bool operator<(const cpg_match_info& lv, const cpg_match_info& rv);

 












