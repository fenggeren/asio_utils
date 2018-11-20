//
//  GateSession.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "GateSession.hpp"
#include "MSKernel.hpp"
#include <CPG/CPGHeader.h>

bool GateSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                           const void* buffer, const PacketHeader& header)
{
    switch (header.type)
    {
        case kJoinMatchRQ:
        {
            MSKernel::instance().joinMatchRQ(session, buffer, header);
            break;
        }
        case kUnjoinMatchRQ:
        {
            MSKernel::instance().unjoinMatchRQ(session, buffer, header);
            break;
        }
            
        default:
            break;
    }
    return true;
}
