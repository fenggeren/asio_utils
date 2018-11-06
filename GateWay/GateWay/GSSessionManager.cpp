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
#include <CPG/CPGServerDefine.h>
#include "G2CSession.hpp"
#include "G2MSession.hpp"
#include "G2LSession.hpp"

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

std::shared_ptr<ClientSession>
GSSessionManager::createConnectorSession(uint8 type)
{
    if (type == ServerType_CentralServer)
    {
        return std::make_shared<G2CSession>();
    }
    else if (type == ServerType_MatchServer)
    {
        return std::make_shared<G2MSession>();
    }
    else if (type == ServerType_LoginServer)
    {
        return std::make_shared<G2LSession>();
    }
    else
    {
        return TCPSessionManager::createConnectorSession(type);
    }
    
}
