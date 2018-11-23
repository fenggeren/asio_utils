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
#include <CPG/CPGHeader.h>
#include <CPG/Third/json/json.hpp>
#include <CPG/DB/SQLConnector.hpp>

class CPGMatchCreateFactory
{
public:
    
    CPGMatchCreateFactory();
    
    std::map<int, std::shared_ptr<CPGMatchProfile>>
    initialize();
    
    // 数据库加载
    std::list<std::shared_ptr<CPGMatchProfile>> loadFromDB();
    // json 创建
    std::shared_ptr<CPGMatchProfile> createMatch(const std::string& cnt);
    std::shared_ptr<CPGMatchProfile> createMatch(const nlohmann::json& json);
    
    std::list<std::shared_ptr<CPGMatchProfile>> loadFromFile();
private:
    
    std::pair<bool, int> checkValidAndStore(std::shared_ptr<CPGMatchProfile> profile);
    
    
    int queryMid(std::shared_ptr<CPGMatchProfile> profile);
private:
    
    SQLConnector connector_;
};









