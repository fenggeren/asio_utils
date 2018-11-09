//
//  CPGToCentralSession.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CPGToCentralSession.hpp"
#include "../../Net/Util/NetPacket.hpp"
#include "../../Net/Util/ParseProto.hpp"
#include "../../Net/logging/Logging.hpp"
#include "../../Net/ServiceKernel.hpp"
#include "CPGServerDefine.h"
#include <CPG/CPGToCentral.pb.h>

using namespace logging;

void CPGToCentralSession::defaultMessageCallback(
                                              const std::shared_ptr<TCPSession>& session,
                                              DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type)
        {
            case kServerRegistRS:
            {
                serverRegistRS(buffer, header->size);
                break;
            }
            case kServerNewServicesNotify:
            {
                newServicesNotify(buffer, header->size);
                break;
            }
            default:
            {
                handlerMsg(session, buffer, *header);
                break;
            }
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}

void CPGToCentralSession::newServicesNotify(const void* data, int len)
{
    CPGToCentral::NewConnServiceNotify rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        for(auto& connsvr : rs.connservers())
        {
            serviceKernel().addNewConnect(connsvr.type(), connsvr.port(), connsvr.sid(), connsvr.ip());
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}
void CPGToCentralSession::serverRegistRS(const void* data, int len)
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
                serviceKernel().addNewConnect(connsvr.type(), connsvr.port(), connsvr.sid(), connsvr.ip());
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

