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
    GSKernel::instance().removeConnectService(uuid());
}

void G2CSession::sendInitData()
{
    sendRegisterData(SessionManager);
}


bool G2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kLoginRS:
            {
                serverLoginRS(buffer, header.size);
                break;
            }
        case kServerAllMatchDistributeNotify:
        {
            GSKernel::instance().distibuteMatchesNotify(buffer, header);
            break;
        }
            default:
                break;
    }
    return true;
}
ServiceKernel& G2CSession::serviceKernel()
{
    return GSKernel::instance();
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

