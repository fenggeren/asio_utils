//
//  G2LSession.cpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "G2LSession.hpp"
#include <CPG/CPGClient.pb.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "GSKernel.hpp"
#include "GSSessionManager.hpp"
#include <CPG/CPGServerDefine.h>

using namespace fasio::logging;



void G2LSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                       DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kLoginRS:
            {
                loginRS(buffer, header->size);
                break;
            }
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}


void G2LSession::loginRS(const void* data, int len)
{
    CPGClient::LoginRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        // 转发消息给 客户端
        SessionManager.sendMsgToSession(rs.logicid(), rs, kLoginRS);
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}
