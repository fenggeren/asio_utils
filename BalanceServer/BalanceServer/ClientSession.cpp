//
//  ClientSession.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "ClientSession.hpp"
#include <CPG/CPGClient.pb.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "BSKernel.hpp"
#include "BSSessionManager.hpp"

using namespace logging;

void CBSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                      DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kConnectRQ:
            {
                connectRQ(buffer, header->size);
                break;
            } 
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}


void CBSession::connectRQ(const void* data, int len)
{
    CPGClient::ConnectRQ rq;
    if (fasio::parseProtoMsg(data, len, rq))
    {
        rq.set_logicid(uuid());
        // 转发消息给CentralServer
        SessionManager.sendMsgToSession(0, rq, kConnectRQ, ServerType_Balance_Central);
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}




