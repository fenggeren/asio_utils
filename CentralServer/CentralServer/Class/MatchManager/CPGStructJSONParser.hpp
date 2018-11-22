//
//  CPGStructJSONParser.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//
#pragma once

#include <CPG/CPGMatchDefine.h>
#include <CPG/Third/json/json.hpp>
#include <memory>


namespace nlohmann
{ 
    using nlohmann::json;
    
    void to_json(json& j,
                 const CPGMatchProfile& match);
    void from_json(const json& j, CPGMatchProfile& match);
}

