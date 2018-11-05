//
//  main.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include "MSSessionManager.hpp"
#include "MSKernel.hpp"


int main(int argc, const char * argv[]) {

    MSKernel kernel;
    kernel.start();
    
    return 0;
}
