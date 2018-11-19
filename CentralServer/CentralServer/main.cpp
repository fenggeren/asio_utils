//
//  main.cpp
//  CentralServer
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include <Net/Conv.hpp>
#include "Class/Net/CSKernel.hpp"
#include "Class/MatchManager/CSMatchManager.hpp"
#include <CPG/Util/ServerConfigManager.hpp>

int main(int argc, const char * argv[]) {
    
    fasio::setCurThreadName("main");
    
    CSMatchManager::instance().initialize();

    auto manager = ServerConfigManager::instance();
    manager.setType(ServerType_MatchServer);
    ServerNetConfig config;
    if (manager.configForType(ServerType_CentralServer, config))
    {
        if (config.infos.size() == 1)
        {
            CSKernel::instance().start(config.infos.front());
        }
    }
    
    return 0;
}










