//
//  CSMatchDistDefine.h
//  CentralServer
//
//  Created by huanao on 2018/11/13.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once


struct MatchDisService
{
    int  sid{-1};
    int  loaded{0};
};


struct ChangedService
{
    enum Type { Added, Update, Removed, };
    Type type;
    MatchDisService service;
    time_t stamp;
};



inline bool operator<(const MatchDisService& lv,const MatchDisService& other)
{
    return lv.sid < other.sid;
}
