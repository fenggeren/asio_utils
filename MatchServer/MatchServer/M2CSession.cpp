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
#include <CPG/Util/ServerConfigManager.hpp>
#include <Net/Conv.hpp>
#include <algorithm>

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
    
    sendRegisterData(SessionManager);
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
        rq.add_mids(mid);
    }
    MSKernel::instance().sendMsg(shared_from_this(), rq,
                                 kCheckMatchDistributeRQ,uuid());
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









