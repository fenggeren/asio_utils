//
//  CSMatchManager.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/2.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSMatchManager.hpp"
#include <string>
#include <iostream>

std::string matchConfig =
#if 1
"";
#else
R"(
{
    \"patterns\": [{
    \"date_duration\": \"20181115\",
    \"weeks\": \"1,2,3,4\",
    \"times\": \"14:00, 15:00, 17:00, 19:00\",
    \"generate_date\": \"20180327\",
    \"generate_time_cycle\": 7,
    \"type\": 2,
    \"rule_id\": 40005
},
{
    \"date_duration\": \"20180427-20181115\",
    \"weeks\": \"1,2,3,4,5,6,7\",
    \"times\": \"10:30, 11:00, 11:15, 11:45, 12:05, 12:30, 14:00, 14:20, 14:40,  15:00, 15:20, 15:40, 16:00, 16:10, 16:20, 16:30, 16:40, 17:00, 17:10, 17:20, 17:40, 17:50, 19:00\",
    \"generate_date\": \"20180327\",
    \"generate_time_cycle\": 7,
    \"type\": 2,
    \"rule_id\": 40004
},
{
    \"date_duration\": \"20180427-20181115\",
    \"weeks\": \"1,2,3,4,5,6,7\",
    \"times\": \" 18:50\",
    \"generate_date\": \"20180327\",
    \"generate_time_cycle\": 7,
    \"type\": 1,
    \"rule_id\": 40004
}
],
\"rules\": [{
\"rule_id\": 40004,
\"match_name\": \"400多资格测试赛\",
\"start_condition\": 0,
\"min_players_amount\": 6,
\"max_players_amount\": 2000,
\"delay_join\": 10,
\"start_blind_level\": 1,
\"entryfee_coin\": 40000,
\"entry_condition\": \"40000参赛积分(可重进)\",
\"speed\": 15,
\"init_chips_amount\": 10000,
\"increase_blind_time\": 480,
\"reward_type\": 4,
\"bonus_id\": 40,
\"guarantee_person\": 50,
\"match_type\": 2,
\"description\": \"\",
\"end_match_type\": 0,
\"end_match_param\": 0,
\"extra\": \"\"
}],
\"matches\": [{
\"match_name\": \"5000保底100积分赛\",
\"begin_date\": \"2018-05-01 21:30:00\",
\"start_condition\": 0,
\"min_players_amount\": 6,
\"max_players_amount\": 2000,
\"delay_join\": 10,
\"start_blind_level\": 1,
\"entryfee_coin\": 10000,
\"entry_condition\": \"10000参赛积分(可重进)\",
\"speed\": 15,
\"init_chips_amount\": 10000,
\"increase_blind_time\": 480,
\"reward_type\": 2,
\"bonus_id\": 27,
\"guarantee_person\": 50,
\"match_type\": 2,
\"end_match_type\": 1,
\"end_match_param\": 50528257,
\"extra\": \"\"
}]
}
)";
#endif

inline bool operator<(const MatchDisService& lv,const MatchDisService& other)
{
    return lv.sid < other.sid;
}


// 初始化
void CSMatchManager::initialize()
{
    using namespace nlohmann;
    
    match_factory_.load_from_file();
    match_factory_.create_all_matches();
}

// 加载所有比赛
void CSMatchManager::loadAllMatches()
{
    
}
// 更新所有配置比赛
void CSMatchManager::updateConfigMatches()
{
    
}



void
CSMatchManager::updateMatchService(const std::shared_ptr<ServerInfo>& service)
{
    MatchDisService ms{service->sid, service->loaded};
    auto iter = matchServices_.find(ms);
    // 服务已存在，根据负载是否从新分配比赛
    if (iter != matchServices_.end())
    {
        // 遍历所有比赛服务，是否有负载超出预期的。
    }
    else
    {
        
    }
}

void
CSMatchManager::removeMatchService(int sid)
{ 
}















