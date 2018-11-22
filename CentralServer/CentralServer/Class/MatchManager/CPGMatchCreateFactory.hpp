//
//  CPGMatchCreateFactory.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <list>
#include <CPG/CPGHeader.h>

class CPGMatchCreateFactory
{
public:
    
    // 数据库加载
    std::list<CPGMatchProfile> loadFromDB();
    
    // json 创建
    bool createMatch(const std::string& cnt, CPGMatchProfile& match);
    
    
private:
    
};









