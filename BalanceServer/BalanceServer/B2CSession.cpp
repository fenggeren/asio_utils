//
//  B2CSession.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "B2CSession.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGClient.pb.h>
#include <CPG/CPGServerDefine.h>
#include "BSSessionManager.hpp"
#include "BSKernel.hpp"
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>

using namespace fasio::logging;


void B2CSession::onClose()
{
    
}

void B2CSession::sendInitData()
{
    CPGToCentral::ServerRegisterRQ rq;
    rq.set_type(ServerType_BalanceServer);
    rq.set_port(7835);
    rq.set_sid(0); 
    rq.set_ip("127.0.0.1");
    
    SessionManager.sendMsgToSession(shared_from_this(), rq,
                                    kServerRegistRQ, ServerType_CentralServer);
}

void B2CSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                        DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kServerRegistRS:
            {
                serverRegistRS(buffer, header->size);
                break;
            }
            case kConnectRS:
            {
                connectRS(buffer, header->size);
                break;
            }
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}

void B2CSession::serverRegistRS(const void* data, int len)
{
    CPGToCentral::ServerRegisterRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        if (rs.result() == 0)
        {
            setLogicID(rs.sid()); // 设置server id
            LOG_MINFO << " server id: " << rs.sid();
            
        }
        else
        {
            LOG_ERROR << " gs regist failure result: " << rs.result();
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}

void B2CSession::connectRS(const void* data, int len)
{
    CPGClient::ConnectRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        if (rs.result() == 0)
        {
            SessionManager.sendMsgToSession(rs.logicid(), data, len, kConnectRS);
        }
        else
        {
            LOG_ERROR << " bs connectrs failure result: " << rs.result();
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}

