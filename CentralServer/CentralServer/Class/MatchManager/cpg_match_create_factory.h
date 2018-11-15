// #ifndef cpg_match_create_factory_h_
// #define cpg_match_create_factory_h_
#pragma once

#include <memory>
#include <list>
#include <map>
#include <set>

#include <CPG/cpg_match_struct_define.h>
#include <CPG/Third/json/json.hpp>


// 比赛创建
/*
一：
1. 从json文件加载 创建规则，并存储到redis
2. 通过从redis获取所有的创建规则
3. 根据规则创建所有比赛
二：
直接添加比赛
*/

// 线程不安全，必须在逻辑线程处理！！！
class cpg_match_create_factory
{
public:
    
    static cpg_match_create_factory& instance()
    {
        static cpg_match_create_factory factory;
        return factory;
    }
    
    // redis加载所有比赛；
    // 规则和pattern创建完比赛后，其数据对于比赛就无关了！
    // 也不存储到redis
    // 比赛的rule_id和其match_id 一致化！
    // @return:  存储的比赛
    std::list<std::shared_ptr<cpg_match_info>>
    load_from_reids();

    // file 加载所有, 并存储到redis
    // @return: 添加的新的比赛
    std::list<std::shared_ptr<cpg_match_info>>
    load_from_file();
    
    std::list<std::shared_ptr<cpg_match_info>>
    load_from_json(const nlohmann::json& json);
    
    // 根据模式 创建所有格式比赛
    std::list<std::shared_ptr<cpg_match_info>>
    create_all_matches();

    // 通过 规则 json 创建比赛
    std::list<std::shared_ptr<cpg_match_info>>
    create_match(const cpg_match_generate_date& date_patten,
                      const cpg_match_rule& rule);

    const std::map<int, std::shared_ptr<cpg_match_info>>&
    get_all_matches() const { return matches_; }

    std::shared_ptr<cpg_match_info>
    create_match(const nlohmann::json& json, int begin_time);
    
private:

    bool check_match_valid(std::shared_ptr<cpg_match_info> info);

    //  根据pattern 获取所有的创建比赛时间列表
    std::list<int>
    get_all_begin_date_time(const cpg_match_generate_date& generate_date);

    int get_next_match_id();
    int get_next_rule_id();

private:

    std::map<int, cpg_match_rule> rules_;
    std::list<cpg_match_generate_date> patterns_;

    std::map<int, std::shared_ptr<cpg_match_info>> matches_;

};

 
