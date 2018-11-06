//
//  LoginSession.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "LoginSession.hpp"
#include <Net/Util/NetPacket.hpp>
#include "CSKernel.hpp"

using namespace fasio;

void LoginSession::onClose()
{
    CSKernel::instance().removeService(logicID());
}

void LoginSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session, DataBuffer*const data)
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
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}
