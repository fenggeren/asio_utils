//
//  ClientSession.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "ClientSession.hpp"
#include <CPG/CPGClientServer.pb.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "BSKernel.hpp"
#include "BSSessionManager.hpp"

using namespace logging;



bool CBSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    // 消息转发，不做解析处理
    if (header.type >= BSTransToCSFromCP_Begin &&
        header.type <= BSTransToCSFromCP_End)
    {
        auto h = header;
        h.extraID = uuid();
        
        BSKernel::instance().transToCS(buffer, h);
    }
    else
    {
        switch (header.type) {
            default:
                break;
        }
    }

    return false;
}

 




