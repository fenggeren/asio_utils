//
//  GateSession.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "GateSession.hpp"
#include <CPG/CPGServer.pb.h>
#include <CPG/CPGClientServer.pb.h>
#include <CPG/CPGHeader.h>
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
        case kClientLoginRQ:
        {
            loginRQ(buffer, header);
            break;
        }
        default:
            break;
    }
    return true;
}

void GateSession::loginRQ(const void* data, const PacketHeader& header)
{
    LOG_MINFO << " csid: " << header.extraID;
    CPGClientServer::LoginRS rs;
    // token 校验
    CPGClientServer::LoginRQ rq;
    if (fasio::parseProtoMsg(data, header.size, rq))
    {
        rs.set_result(0);
    }
    else
    {
        rs.set_result(-1);
        LOG_ERROR << " cant parse proto msg len: " << header.size
        << " sessionID: " << uuid();
    }
    SessionManager.sendMsg(shared_from_this(), rs.SerializeAsString().data(),
                        {kClientLoginRS, rs.ByteSize(), header.extraID});
}






