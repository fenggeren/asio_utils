//
//  main.cpp
//  ClientRobots
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include <iostream>
#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSessionFactory.h>
#include "Robot.hpp"
#include "RobotManager.hpp"


using namespace fasio;

int main(int argc, const char * argv[]) {
    
    gRobotManager.setRobotNum(1);
    gRobotManager.start();
    return 0;
}
