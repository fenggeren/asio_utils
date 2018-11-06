//
//  ClientSession.cpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "ClientSession.hpp"
#include <CPG/CPGClient.pb.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "GSKernel.hpp"
#include "GSSessionManager.hpp"

using namespace fasio::logging;

void CGSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                            DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kLoginRQ:
            {
                loginRQ(buffer, header->size);
                break;
            }
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}


void CGSession::loginRQ(const void* data, int len)
{
    CPGClient::LoginRQ rq;
    if (fasio::parseProtoMsg(data, len, rq))
    {
        rq.set_logicid(uuid());
        // 转发消息给CentralServer
        SessionManager.sendMsgToSession(0, rq, kLoginRQ, ServerType_Gate_Login);
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}





