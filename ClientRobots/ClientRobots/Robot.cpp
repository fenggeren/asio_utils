//
//  Robot.cpp
//  ClientRobots
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "Robot.hpp"
#include "RobotManager.hpp"

void Robot::connect()
{
    gRobotManager.postConnect(shared_from_this());
}