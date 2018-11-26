//
//  DBWrapperHandler.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include "../DB/DBActiveWrapper.hpp"

class DBWrapperHandler
{
public:
    
    void cancelMatch(int mid);
    // 更新比赛状态
    void updateMatchState(int state);
    
    
private:
    
};


