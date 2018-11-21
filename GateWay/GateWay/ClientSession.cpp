//
//  ClientSession.cpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "ClientSession.hpp"
#include <CPG/CPGClientServer.pb.h>
#include <CPG/CPGHeader.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "GSKernel.hpp"
#include "GSSessionManager.hpp"

using namespace fasio::logging;
bool CGSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    // 消息转发，不做解析处理
    if (header.type >= GSTransFromCP_Begin &&
        header.type <= GSTransFromCP_End)
    {
        auto netHeader = header;
        // 消息转发，不做解析处理
        if (header.type >= GSTransToCSFromCP_Begin &&
            header.type <= GSTransToCSFromCP_End)
        {
            netHeader.extraID = uuid();
            GSKernel::instance().transToCS(buffer, netHeader);
        }
        else if (header.type >= GSTransToMSFromCP_Begin &&
                     header.type <= GSTransToMSFromCP_End)
        {
            netHeader.extraID = uuid();
            GSKernel::instance().transToMS(buffer, netHeader, header.extraID);
        }
        else if (header.type >= GSTransToLSFromCP_Begin &&
                    header.type <= GSTransToLSFromCP_End)
        {
            netHeader.extraID = uuid();
            GSKernel::instance().transToLS(buffer, netHeader);
        }
    }
    else
    {
        LOG_ERROR << "unrecognize msg: " << header.type;
    }
//    updateHeartBeat();
    return true;
}


void CGSession::loginRQ(const void* data, int len)
{
    LOG_MINFO << " ";
    GSKernel::instance().transToLS(data, {kClientLoginRQ, len, static_cast<int32>(uuid())});
}












