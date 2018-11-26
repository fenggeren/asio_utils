//
//  CPGMatchStructHelper.hpp
//  WebServer
//
//  Created by huanao on 2018/11/23.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <map>
#include <string>
#include "../CPGMatchDefine.h"
#include "../Third/json/json.hpp"

std::map<std::string, std::string>
structConvertMap(const CPGMatchProfile& detail);
void mapConvertStruct(std::map<std::string, std::string>& dict,
                        CPGMatchProfile& detail);



namespace nlohmann
{
    using nlohmann::json;
    
    void to_json(json& j, const CPGMatchProfile& json);
    void from_json(const json& j, CPGMatchProfile& json);
}






