//
//  GateSession.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "GateSession.hpp"

bool GateSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                           const void* buffer, const PacketHeader& header)
{
    return true;
}
