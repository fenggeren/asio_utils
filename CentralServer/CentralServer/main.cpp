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

int main(int argc, const char * argv[]) {
    
    fasio::setCurThreadName("main");
    
    CSMatchManager::instance().initialize();
    
    CSKernel::instance().start();
    
    return 0;
}
