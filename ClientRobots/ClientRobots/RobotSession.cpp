//
//  RobotSession.cpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "RobotSession.hpp"
#include <CPG/CPGClient.pb.h>
#include "RobotSessionManager.hpp"
#include <CPG/CPGServerDefine.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "RobotManager.hpp"
#include "Robot.hpp"

// sock????
// ??????GateServer??????
void RobotSession::sendInitData() 
{
    setLogicID(uuid());
    if (firstConnect_)
    {
        // ????gs????
        CPGClient::ConnectGateRQ rq;
        SessionManager.sendMsgToSession(shared_from_this(), rq, kClientConnectRQ,ServerType_GateServer);
    }
}

void RobotSession::onClose() 
{
    firstConnect_ = true;
}


void RobotSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session, DataBuffer*const data)
{
   while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kClientConnectRQ:
            { 
                connectRS(buffer, header->size);
                break;
            } 
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}


void RobotSession::connectRS(const void* data, int len)
{
    CPGClient::ConnectGateRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        firstConnect_ = false;
        // ????  ????session???????session
        auto robot = gRobotManager.getRobot(logicID());
        robot->setIP(rs.ip());
        robot->setPort(rs.port());
        robot->connect();
    }
    else 
    {

    }
}