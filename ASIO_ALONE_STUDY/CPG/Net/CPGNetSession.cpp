//
//  CPGNetSession.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/8.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "CPGNetSession.hpp"
#include "../../Net/FASIO.hpp"
#include "CPGServerDefine.h"
#include "../CPGServer.pb.h"
#include "../CPGClientServer.pb.h"
#include "../MessageTypeDefine.h"

using namespace logging;

CPGServerSession::CPGServerSession(const SocketPtr& sock, const std::string& name)
:TCPSession(sock, name)
{
    messageCallback_ = std::bind(&CPGServerSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
}

void CPGServerSession::defaultMessageCallback(
      const std::shared_ptr<TCPSession>& session,
                           DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        if (header->type == kHeartBeatRQ)
        {
            heartBeatRQ();
        }
        else
        {
            handlerMsg(session, buffer, *header);
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}

void CPGServerSession::heartBeatRQ()
{
    updateHeartBeat();
    CPGClientServer::BeatHeartRS rs;
    rs.set_type(type());
    PacketHeader header{kHeartBeatRS, rs.ByteSize(), 0};
    addMore(&header, kPacketHeaderSize);
    send(rs.SerializeAsString());
}


CPGClientSession::CPGClientSession()
:ClientSession()
{
    messageCallback_ = std::bind(&CPGClientSession::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2);
}

void CPGClientSession::defaultMessageCallback(
    const std::shared_ptr<TCPSession>& session,
                            DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        if (header->type == kHeartBeatRS)
        {
            updateHeartBeat();
            LOG_DEBUG << " kHeartBeatRS ";
        }
        else
        {
            handlerMsg(session, buffer, *header);
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}







