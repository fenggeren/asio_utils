//
//  main.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include "LSKernel.hpp"

int main(int argc, const char * argv[]) {
    
    LSKernel::instance().start();
    
    return 0;
}
