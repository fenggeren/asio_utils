//
//  GateSession.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "GateSession.hpp"
#include <Net/Util/NetPacket.hpp>
#include "CSKernel.hpp"

using namespace fasio;

void GateSession::onClose()
{
    CSKernel::instance().removeService(logicID());
}

bool GateSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kServerRegistRQ:
        {
            CSKernel::instance().serverRegistRQ(session, buffer, header.size);
            break;
        }
        case kMatchListRQ:
        {
            CSKernel::instance().matchListRQ(session, buffer, header);
            break;
        }
        default:
            break;
    }
    return true;
}










