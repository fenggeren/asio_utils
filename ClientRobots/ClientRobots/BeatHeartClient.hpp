//
//  BeatHeatClient.hpp
//  ClientRobots
//
//  Created by huanao on 2018/11/21.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <memory>
#include <Net/FASIO.hpp>
#include <google/protobuf/message.h>


/*
 心跳包发送接受
 超时10s，发送一次心跳包。
 超时15s，就认为是断开连接。
 */

class BeatHeartClient
{
public:
     
    time_t beatHeartTimer() const
    {  return beatHeartTimer_; }
    
    void updateBeatHeart()
    {
        beatHeartTimer_ = time(NULL);
    }
    
    bool overtime(time_t cur)
    {
        return cur - beatHeartTimer_ >= 15;
    }
    bool beatHeart(time_t cur)
    {
        return cur - beatHeartTimer_ >= 10;
    }
    
public:
    static fasio::NetPacketPtr heartBeatMessage();
private:
    time_t beatHeartTimer_;
    fasio::NetPacketPtr beatHeartPacket_;
};



















