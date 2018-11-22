//
//  CPGMatchDefine.h
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <string>

struct CPGMatchProfile
{
    // 动态数据
    int currentBlindLevel{0};  // 当前盲注级别  恢复赛使用
    int totalPlayerAmount{0};  // 当前总报名人数
    int remainedPlayerAmount{0}; // 当前剩余人数.
    
    int matchState{0};             // 比赛状态
    int matchID;
    // 静态数据
    int matchType;
    int enterCode;
    int startTime;
    int increaseBlindTime;   //升盲时间
    int operatorTime;         // 思考时间
    int entryfee;              // 报名费
    int entryPool;             // 入池费用.  抽水 = entryfee - entryPool
    int rewardpercent;          // 抽水比例
    int initChipsAmount;        // 初始筹码量
    int minPlayersAmount;
    int maxPlayersAmount;
    int delayJoinLevel;  //延迟报名级别
    int rewardType;
    int startCondition;     // 开赛条件
    int stopCondition;      //截止报名条件
    int finishCondition;     // 结束条件  // 默认决出冠军.
    int finishParam;          // 结束参数  例如 剩余1人。  正常结束：比赛状态必须在截止报名后
    int firstBlindLevel;      // 起始盲注级别
    int bonusID;
    int guaranteePerson;
    int matchAutoCancelSecond;  // 比赛创建后，未开赛，自动截止时间。 只针对自定义比赛
    unsigned long long bankID{0}; // 庄家 官方默认是000000
    std::string matchName;
};





