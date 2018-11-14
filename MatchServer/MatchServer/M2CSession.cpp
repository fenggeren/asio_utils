//
//  M2CSession.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "M2CSession.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGServerDefine.h>
#include "MSSessionManager.hpp"
#include <Net/Util/NetPacket.hpp>
#include <Net/logging/Logging.hpp>
#include <Net/Util/ParseProto.hpp>
#include "MSMatchManager.hpp"
#include "MSKernel.hpp"

using namespace fasio::logging;

void M2CSession::onClose()
{
    MSKernel::instance().removeConnectService(uuid());
}

ServiceKernel& M2CSession::serviceKernel()
{
    return MSKernel::instance();
}


void M2CSession::sendInitData()
{
    CPGToCentral::ServerRegisterRQ rq;
    rq.set_type(ServerType_GateServer);
    rq.set_port(7851);
    rq.set_sid(0);
    rq.set_ip("127.0.0.1");
    rq.set_exportip("127.0.0.1");
    
    SessionManager.sendMsgToSession(shared_from_this(), rq,
                                    kServerRegistRQ, ServerType_CentralServer);
    if (firstConnect_)
    {
        firstConnect_ = false;
    }
    else
    {
        // 重连, check
        checkMatchDistribute();
    }
}

void M2CSession::checkMatchDistribute()
{
    auto matches = MSMatchManager::instance().matches();
    
    CPGToCentral::CheckMatchDistributeRQ rq;
    rq.set_sid(logicID());
    rq.set_type(ServerType_MatchServer);
    for(auto mid : matches)
    {
        rq.add_mid(mid);
    }
    PacketHeader header{kCheckMatchDistributeRQ, rq.ByteSize(), static_cast<int32>(uuid())};
    SessionManager.sendMsg(shared_from_this(), rq.SerializeAsString().data(), header);
}

bool M2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                        const void* buffer,
                        const PacketHeader& header)
{
    switch (header.type) {
        case kCheckMatchDistributeRS:
        {
            MSKernel::instance().checkMatchDistributeRS(buffer, header);
            break;
        }
        case kServerMatchDistributeNotify:
        {
            MSKernel::instance().distibuteMatchesNotify(buffer, header);
            break;
        }
        default:
            break;
    }
    return true;
}









