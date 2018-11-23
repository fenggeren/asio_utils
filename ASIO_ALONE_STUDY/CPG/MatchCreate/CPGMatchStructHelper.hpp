//
//  CPGMatchStructHelper.hpp
//  WebServer
//
//  Created by huanao on 2018/11/23.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <map>
#include <string>
#include <CPG/CPGMatchDefine.h>


std::map<std::string, std::string>
structConvertMap(const CPGMatchProfile& detail);
void mapConvertStruct(std::map<std::string, std::string>& dict,
                        CPGMatchProfile& detail);


