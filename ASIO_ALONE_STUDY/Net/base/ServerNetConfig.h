//
//  ServerNetConfig.h
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

struct ServerNetConfig
{
    struct ServerNetInfo
    {
        int type{0};
        unsigned short port{0};
        std::string ip;
    };
    
    struct ServerInfo
    { 
        std::string ip;
        std::vector<int>              connectTypes;
        std::vector<ServerNetInfo> listenInfos;
        std::vector<ServerNetInfo> connectInfos; // 计算得来
    };
    int type{0};
    std::string name;
    std::vector<ServerInfo> infos;
};
