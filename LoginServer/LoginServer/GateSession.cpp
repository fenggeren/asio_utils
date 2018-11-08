//
//  GateSession.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "GateSession.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGClient.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <Net/Util/NetPacket.hpp>
#include <Net/logging/Logging.hpp>
#include "CPGServerDefine.h"
#include "LSSessionManager.hpp"

using namespace fasio::logging;

bool GateSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kLoginRQ:
        {
            loginRQ(buffer, header.size);
            break;
        }
        default:
            break;
    }
    return true;
}

void GateSession::loginRQ(const void* data, int len)
{
    CPGClient::LoginRS rs;
    // token 校验
    CPGClient::LoginRQ rq;
    if (fasio::parseProtoMsg(data, len, rq))
    {
        rs.set_result(0);
        rs.set_logicid(rq.logicid());
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
    SessionManager.sendMsgToSession(shared_from_this(), rs, kLoginRS);
}






