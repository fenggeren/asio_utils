//
//  CPGMatchCreateFactory.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <list>
#include <map>
#include "../CPGHeader.h"
#include "../Third/json/json.hpp"
#include "../DB/SQLConnector.hpp"

class CPGMatchCreateFactory
{
public:
    
    CPGMatchCreateFactory();
    
    std::map<int, std::shared_ptr<CPGMatchProfile>>
    initialize();
    
    // 数据库加载
    std::list<std::shared_ptr<CPGMatchProfile>> loadFromDB();
 
    std::list<std::shared_ptr<CPGMatchProfile>> loadFromFile();
    
    // json 创建
    std::shared_ptr<CPGMatchProfile> createMatch(const std::string& cnt);
    std::shared_ptr<CPGMatchProfile> createMatch(const nlohmann::json& json);
    std::shared_ptr<CPGMatchProfile>
        createMatch(std::shared_ptr<CPGMatchProfile> profile);
  
    
    
    
    
    static std::pair<bool, int>
    checkValidMatch(CPGMatchProfile& profile,
                       SQLConnector& connector);
    
    static int queryMatchMid(CPGMatchProfile& profile,
                        SQLConnector& connector);
    
    static int createAndStoreMatch(CPGMatchProfile& profile,
                                   SQLConnector& connector);
private:
    
    std::pair<bool, int>
    checkValidAndStore(std::shared_ptr<CPGMatchProfile> profile);
    
    int queryMid(std::shared_ptr<CPGMatchProfile> profile);

    
private:
    
    SQLConnector connector_;
};









