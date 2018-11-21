//
//  BeatHeatClient.cpp
//  ClientRobots
//
//  Created by huanao on 2018/11/21.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "BeatHeartClient.hpp"
#include <CPG/CPGClientServer.pb.h>
#include <CPG/CPGHeader.h>

fasio::NetPacketPtr BeatHeartClient::heartBeatMessage()
{
    static fasio::NetPacketPtr beatHeartPacket = nullptr;
    if (!beatHeartPacket)
    {
        CPGClientServer::BeatHeartRQ beat;
        beat.set_type(ServerType_Client);
        PacketHeader header{kHeartBeatRQ, beat.ByteSize(),0};
        beatHeartPacket = std::make_shared<NetPacket>
        (beat.SerializeAsString().data(), header);
    }
    return beatHeartPacket;
    
}








