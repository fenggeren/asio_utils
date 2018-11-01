//
//  main.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include "MSSessionManager.hpp"


int main(int argc, const char * argv[]) {
    
    MSSessionManager manager;
    manager.start();
    
    return 0;
}
