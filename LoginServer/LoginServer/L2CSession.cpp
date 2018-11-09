//
//  L2CSession.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "L2CSession.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGServerDefine.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/logging/Logging.hpp>
#include <Net/Util/ParseProto.hpp>
#include "LSSessionManager.hpp"
#include "LSKernel.hpp"

using namespace fasio::logging;

void L2CSession::sendInitData()
{
    CPGToCentral::ServerRegisterRQ rq;
    rq.set_type(ServerType_GateServer);
    rq.set_port(7831);
    rq.set_sid(0);
    rq.set_ip("127.0.0.1");
    rq.set_ip("127.0.0.1");
    
    SessionManager.sendMsgToSession(shared_from_this(), rq,
                                    kServerRegistRQ, ServerType_CentralServer);
}


void L2CSession::onClose()
{
    LSKernel::instance().removeConnectService(uuid());
}

bool L2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        default:
            break;
    }
    return true;
}
ServiceKernel& L2CSession::serviceKernel()
{
    return LSKernel::instance();
}






