//
//  BalanceSession.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "BalanceSession.hpp"
#include <Net/Util/NetPacket.hpp>
#include "CSKernel.hpp"
#include <Net/Util/ParseProto.hpp>
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGClient.pb.h>
#include <CPG/CPGServerDefine.h>
#include "CSSessionManager.hpp"

using namespace fasio::logging;
using namespace fasio;

void BalanceSession::onClose()
{
    CSKernel::instance().removeService(logicID());
}


bool BalanceSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kServerRegistRQ:
        {
            CSKernel::instance().serverRegistRQ(session, buffer, header.size);
            break;
        }
        case kConnectRQ:
        {
            CSKernel::instance().requestBestGateServer(shared_from_this(),
                                                       buffer, header);
            break;
        }
        default:
            break;
    }
    return true;
}




