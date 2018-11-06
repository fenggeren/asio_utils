//
//  main.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include "BSKernel.hpp"

int main(int argc, const char * argv[]) {
    
    BSKernel::instance().start();
    
    return 0;
}
