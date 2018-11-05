//
//  G2CSession.cpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "G2CSession.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGServerDefine.h>
#include "GSSessionManager.hpp"
#include "GSKernel.hpp"
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>

using namespace fasio::logging;

void G2CSession::onClose()
{
    
}

void G2CSession::sendInitData()
{
    CPGToCentral::ServerRegisterRQ rq;
    rq.set_type(ServerType_GateServer);
    rq.set_port(7890);
    rq.set_sid(0);
    rq.set_ip("127.0.0.1");
    rq.set_ip("127.0.0.1");
    
    SessionManager.sendMsgToSession(shared_from_this(), rq,
                                    kServerRegistRQ, ServerType_GateServer);
}


void G2CSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
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
            case kServerLoginRS:
            {
                serverLoginRS(buffer, header->size);
                break;
            }
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}

void G2CSession::serverRegistRS(const void* data, int len)
{
    CPGToCentral::ServerRegisterRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        if (rs.result() == 0)
        {
            setLogicID(rs.sid()); // 设置server id
            LOG_MINFO << " server id: " << rs.sid();
            
            for(auto& connsvr : rs.connservers())
            {
                GSKernel::instance().addNewConnect(connsvr.type(), connsvr.port(), connsvr.sid(), connsvr.ip());
            }
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
void G2CSession::serverLoginRS(const void* data, int len)
{
    CPGToCentral::ServerLoginRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        if (rs.result() == 0)
        {
            // 转发消息给客户端  exportIP & port & sid
            LOG_MINFO << rs.DebugString();
        }
        else
        {
            LOG_ERROR << " gs loginrs failure result: " << rs.result();
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}

