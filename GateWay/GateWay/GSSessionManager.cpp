//
//  GSSessionManager.cpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "GSSessionManager.hpp"
#include <Net/logging/Logging.hpp>
#include <Net/Util/ParseProto.hpp>
using namespace fasio::logging;


void GSSessionManager::transToMatchServer(int32 mid, const google::protobuf::Message& msg, int32 msgType)
{
    // 根据比赛 mid 获取对应的 G2MSession!
    auto iter = mid2MatchServers_.find(mid);
    if (iter != mid2MatchServers_.end())
    {
        PacketHeader header{msgType, msg.ByteSize()};
        iter->second->addMore(&header, kPacketHeaderSize);
        iter->second->send(msg.SerializeAsString());
    }
    else
    {
        LOG_ERROR << " not found matchserver for mid: " << mid;
    }
}
