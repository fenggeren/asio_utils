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


void BalanceSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                            DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kServerRegistRQ:
            {
                CSKernel::instance().serverRegistRQ(session, buffer, header->size);
                break;
            }
            case kConnectRQ:
            {
                CSKernel::instance().requestBestGateServer(shared_from_this(), buffer, header->size); 
                break;
            }
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}






