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

void G2LSession::onClose()
{
    unenableRetry();
    GSKernel::instance().removeServiceSession(uuid());
}


bool G2LSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    if (header.type >= GSTransToLSFromCP_Begin &&
        header.type <= GSTransToLSFromCP_End)
    {
        GSKernel::instance().transToClient(buffer, header);
    }
    else
    {
        switch (header.type) {
            default:
                break;
        }
    }
    return true;
} 

void G2LSession::loginRS(const void* data, const PacketHeader& header)
{
    LOG_MINFO << " ";
    SessionManager.sendMsg(header.extraID, data, header);
}






