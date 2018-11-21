//
//  StructDefine.h
//  ClientRobots
//
//  Created by huanao on 2018/11/20.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once




struct MatchBriefInfo
{
    int   mid ;
    int   type;
    int   time;
    int   totalPlayer;
    int   remainedPlayer;
    int   state;
    int   curLevel;
    int   entryfee;
};

namespace fasio
{
    class ClientSession;
}

struct UserInfo
{
    int uid;
    std::shared_ptr<fasio::ClientSession> session;
};




