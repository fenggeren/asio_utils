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


using namespace fasio::logging;

void C2BSession::onClose()
{
    
}

void C2BSession::sendInitData()
{
    //向 balance发送连接请求。 返回合适的GateServer配置信息
    CPGClient::ConnectRQ rq;
    rq.set_logicid(uuid());
    SessionManager.sendMsgToSession(shared_from_this(), rq, kConnectRQ, ServerType_BalanceServer);
}


void C2BSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                        DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kConnectRS:
            {
                connectRS(buffer, header->size);
                break;
            }
            default:
            {
                LOG_ERROR << " error msg :" << header->type;
                break;
            }
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}

void C2BSession::connectRS(const void* data, int len)
{
    CPGClient::ConnectRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        auto robot = gRobotManager.getRobot(logicID());
        robot->setIP(rs.ip());
        robot->setPort(rs.port());
        robot->connect();
        forceClose();
    }
    else
    {
        LOG_ERROR << " not parse data to CPGClient::ConnectRS size: " << len;
    }
}

///////////////////////////////////////////////////

void RobotSession::sendInitData()
{
    // 登录请求
    CPGClient::LoginRQ rq;
    rq.set_uid(uuid());
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "Robot-%d",uuid());
    rq.set_token(buf);
    rq.set_logicid(uuid());
    SessionManager.sendMsgToSession(shared_from_this(), rq, kLoginRQ,ServerType_GateServer);
}

void RobotSession::onClose() 
{
}


void RobotSession::defaultMessageCallback(const std::shared_ptr<TCPSession>& session, DataBuffer*const data)
{
   while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type) {
            case kLoginRS:
            {
                loginRS(buffer, header->size);
                break;
            } 
            default:
                break;
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}
void RobotSession::loginRS(const void* data, int len)
{
    
}
