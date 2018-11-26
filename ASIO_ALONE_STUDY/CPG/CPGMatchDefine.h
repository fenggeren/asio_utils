//
//  CPGMatchDefine.h
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <string>

// 28
struct CPGMatchProfile
{
    // 动态数据
    int current_blind_level{1};  // 当前盲注级别  恢复赛使用
    int total_player_amount{0};  // 当前总报名人数
    int remained_player_amount{0}; // 当前剩余人数.
    int match_state{1};             // 比赛状态
    int end_time{0};               // 结束时间.
    int mid{0};
    // 静态数据
    int match_type;
    int enter_code{0};
    int start_time;
    int increase_blind_time;   //升盲时间
    int operator_time;         // 思考时间
    int entryfee;              // 报名费
    int entry_pool{0};             // 入池费用.  抽水 = entryfee - entryPool
    int reward_coin{0};          // 抽水比例
    int init_chips_amount;        // 初始筹码量
    int min_players_amount;
    int max_players_amount;
    int delay_join_level;  //延迟报名级别
    int reward_type;
    int start_condition;     // 开赛条件
    int stop_condition;      //截止报名条件
    int finish_condition{0};     // 结束条件  // 默认决出冠军.
    int finish_param{0};          // 结束参数  例如 剩余1人。  正常结束：比赛状态必须在截止报名后
    int first_blind_level;      // 起始盲注级别
    int bonusid;
    int guarantee_person;
    int match_auto_cancel_second{0};  // 比赛创建后，未开赛，自动截止时间。 只针对自定义比赛
    unsigned long long bankid{0}; // 庄家 官方默认是000000
    std::string name;
};





