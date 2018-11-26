//
//  main.cpp
//  WebServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include "cpg_web_server.hpp"
#include <CPG/MatchHelper/CPGMatchCreateFactory.hpp>


int main(int argc, const char * argv[]) {
    
//    cpg_web_server server;
//    server.start();

    
    CPGMatchCreateFactory factory;
    factory.initialize();
//    factory.loadFromFile();
//    factory.loadFromDB();
    
    return 0;
}
