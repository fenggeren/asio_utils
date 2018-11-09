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
    BSKernel::instance().removeConnectService(uuid());
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

ServiceKernel& B2CSession::serviceKernel()
{
    return BSKernel::instance();
}


bool B2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kConnectRS:
        {
            connectRS(buffer, header);
            break;
        }
        default:
            break;
    }
    return true;
}


void B2CSession::connectRS(const void* data, const PacketHeader& header)
{
    LOG_MINFO << "";
    
    SessionManager.sendMsg(header.extraID, data, header);
}

