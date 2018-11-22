//
//  CPGMatchStructDefine.h
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//
#pragma once

#include <string>
#include <list>
#include <map>
#include "GameDefine.h"

// 比赛创建规则
enum cpg_match_create_type
{
    CreateWeekly = 0,
    CreateDaily,
    CreateDate,
    CreateDetail,  // 直接创建比赛数据，而不是通过模板的形式--！
};


struct cpg_player_account
{
    int user_id;
    int player_heart_time;
    int player_client_version;
    std::string token;
    std::string player_name;
};

struct cpg_match_rule
{
    int rule_id;
    int entryfee_coin;        //报名费
    int init_chips_amount;    //初始筹码量
    int min_players_amount;        //最低开赛人数(按时间开赛时使用)
    int max_players_amount;        //参赛人数上限(按时间开赛时使用)
    int speed;                     //操作秒数
    int increase_blind_time;       //涨盲时间
    int start_blind_level;          //初始盲注级别
    int end_match_type{0};          // 结束比赛的方式 0, 1, 2
    int end_match_param{0};         // 结束比赛的方式 对应的值
    int delay_join;                //延时报名:"比赛开始十分后不可报名"或"盲注进到
    // 第五个级别不可报名"或"盲注进到500/100后不可报名"
    
    /*matchStartConditionType*/
    matchStartConditionType start_condition;        //开赛条件:0,按时间开赛(同时要满足最低开赛人数);1,按人数开赛
    /*matchRewardType*/
    matchRewardType reward_type;    //比赛的奖励类型
    /* ENUM_ROOM_TYPE */
    ENUM_ROOM_TYPE match_type;
    
    // 奖励 id
    int bonus_id;
    // 保底人数
    int guarantee_person;
    
    std::string match_name;
    std::string entry_condition;//字符串描述开赛条件，由服务器制定
    std::string extra{}; //增加新的配置项，以json为格式
};


/* 多日赛 */
struct cpg_match_multiday
{
    //    // 比赛结束 规则  默认 剩余一人结束
    //    struct cpg_match_end_rule_normal
    //    {
    //        virtual bool operator()(int end_value /* match_ptr*/)
    //        {
    //            return false;
    //        }
    //    };
    //    // 时间结束条件  即 盲注级别为基准
    //    struct cpg_match_end_rule_time : cpg_match_end_rule_normal
    //    {
    //        virtual bool operator()(int end_value /* match_ptr*/)
    //        {
    //            return false;
    //        }
    //    };
    
    struct cpg_match_item
    {
        int match_id;  // 比赛id
        int round; // 第几轮
        int end_type; // 结束类型
        int end_value; // 结束数值
    };
    
    int multiday_id;  // 多日赛id
    std::list<cpg_match_item> items;
};


/*
 包含一个rule_id？
 对应规律化 创建 比赛？
 */
struct cpg_match_generate_date
{
    std::string date_duration;       // 比赛创建日期间隔
    std::string weeks;                // 每周的周几，仅周赛使用
    std::string times;                // 比赛创建时间
    std::string generate_date;                // 比赛可见日期
    int generate_time_cycle;           // 比赛创建的周期 单位s，  日赛默认为 24 * 3600
    cpg_match_create_type type;         // 创建类型
    
    // TODO.
    int rule_id;  //  用于找到对应的规则，生成比赛
};

// 比赛创建模板
struct cpg_match_create_template
{
    cpg_match_generate_date generate_date;
    cpg_match_rule rule;
};


struct cpg_match_detail
{
    int match_id{0};
    int begin_date_time{0};            //比赛日期时间
    int rule_id{0};
    
    int total_joined_player{0};        //报名参赛总人数(淘汰后重新报名的计算在内)
    int remained_player{0};        //目前参赛人数(比赛开始前跟上一个字段相等)
    /* ENUM_MTT_MATCH_STATE */
    ENUM_MTT_MATCH_STATE match_state;  //状态:0,比赛未开始不可报名状态,1,比赛未开始可报名状态,2,比赛已经开始可报名状态(延时报名),3,比赛已经开始不可报名状态,4,比赛已经结束.
    //  比赛开始后 实时变化的数据
    int current_blind_level{0};
    
    bool operator<(const cpg_match_detail& other) const
    {
        return begin_date_time < other.begin_date_time;
    }
    bool operator==(const cpg_match_detail& other) const
    {
        return begin_date_time == other.begin_date_time &&
        rule_id == other.rule_id;
    }
};

// 比赛数据
struct cpg_match_info
{
    int match_id{0};
    int begin_date_time{0};            //比赛日期时间
    int rule_id{0};
    
    int total_joined_player{0};        //报名参赛总人数(淘汰后重新报名的计算在内)
    int remained_player{0};        //目前参赛人数(比赛开始前跟上一个字段相等)
    /* ENUM_MTT_MATCH_STATE */
    ENUM_MTT_MATCH_STATE match_state;  //状态:0,比赛未开始不可报名状态,1,比赛未开始可报名状态,2,比赛已经开始可报名状态(延时报名),3,比赛已经开始不可报名状态,4,比赛已经结束.
    //  比赛开始后 实时变化的数据
    int current_blind_level{1};
    
    int entryfee_coin;        //报名费
    int entry_pool_coin;      // 入池量
    int invite_reward_coin;   // 邀请反水奖励
    
    
    int init_chips_amount;    //初始筹码量
    int min_players_amount;        //最低开赛人数(按时间开赛时使用)
    int max_players_amount;        //参赛人数上限(按时间开赛时使用)
    int speed;                     //操作秒数
    int increase_blind_time;       //涨盲时间
    int start_blind_level;          //初始盲注级别
    int end_match_type{0};          // 结束比赛的方式 0, 1, 2
    int end_match_param{0};         // 结束比赛的方式 对应的值
    int delay_join;                //延时报名
    
    /*matchStartConditionType*/
    matchStartConditionType start_condition;        //开赛条件:0,按时间开赛(同时要满足最低开赛人数);1,按人数开赛
    /*matchRewardType*/
    matchRewardType reward_type;    //比赛的奖励类型
    /* ENUM_ROOM_TYPE */
    ENUM_ROOM_TYPE match_type;
    
    // 奖励 id
    int bonus_id;
    // 保底人数
    int guarantee_person;
    
    std::string match_name;
    std::string entry_condition;//字符串描述开赛条件，由服务器制定
    std::string extra{}; //增加新的配置项，以json为格式
};

// 比赛中用户实时 数据;  // 粗粒度
struct cpg_match_player
{
    int                     user_id;
    int                     match_id;
    int                     chips_amount;  //  剩余筹码
    // int                     table_id;
    //    int                     last_chips_amount;
    //    int                     ranking;
    std::string             user_name;
};

// 玩家游戏信息, 细粒度的信息
struct cpg_player_game_info
{
    int      user_id;
    int      match_id;
    int seat_id;
    int cards[2]; // 自己的牌
    
    int cash; // 筹码
    int set;  // 下注
    int last_set;
    
    int action; // fold check call raise allin mark
    int status; // wait game reserve lookon
    bool calc_side_pot;
};

// 游戏状态
struct cpg_game_info
{
    // 分池信息
    struct pool_info
    {
        unsigned long long score;
        bool allin;
        std::list<int> chair_id_list;
        //        std::map<int, int> bytypemap;
    };
    
    int      match_id;
    // ENUM_GAME_STATE
    int state;
    
    int dealer_chir_id; // 庄家椅子号
    int smallbind_chair_id;
    int bigblind_chair_id;
    
    int current_player_chair_id; //当前操作椅子号
    
    int call_amount;  // 每轮跟注额下线
    int minimum_raise; // 每轮加注额下线
    int standard_raise_amount; // 上一个玩家的实际加注金额
    
    int gaming_state;  // flop turn river
    
    int highest_set; // 一轮中下注最多的玩家下注总和
    
    int public_card[5]; // 5张公牌
    
    std::list<pool_info> pool_infos; // 分池
};


// 报名，取消报名，被淘汰


// 参赛信息
struct cpg_match_join_info
{
    enum state
    {
        JOIN = 1,
        UNJOIN, // 取消参赛
        WEEDOUT, // 淘汰
        PROMOTED  // 晋级
    };
    
    int match_id;
    // 自己参赛时， 两值相等-
    int user_id;    // 受邀请人，即参赛者
    int proxy_id;  // 代理id，即邀请者
    // 状态 join => unjoin/eliminate
    state cur_state;
    int join_time;
    int change_time;  // 状态改变时间
};

struct cpg_match_player_reward
{
    int user_id;
    int rank;
    int value;
    CPGBounsType type;
    std::string user_name;
    std::string name;
};















