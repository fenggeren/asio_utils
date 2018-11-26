//
//  ErrorCodeDefine.h
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once


#define kNoneError  0 

// 后台管理
#define kManager_MatchNotFound   10001    // 没有找到对应的比赛
#define kManager_MatchCancel_StartedError 10002 // 取消比赛- 比赛已开赛
#define kCreateMatch_ErrorTimeOut   10101 // 创建比赛，开始时间已过
#define kCreateMatch_ErrorHasExist  10102 // 创建比赛，已存在相同属性<开赛时间、报名费、类型>的比赛













