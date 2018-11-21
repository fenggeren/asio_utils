//
//  RobotSession.cpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "RobotSession.hpp"
#include <CPG/CPGClientServer.pb.h>
#include "RobotSessionManager.hpp"
#include <CPG/CPGServerDefine.h>
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>
#include "RobotManager.hpp"
#include "Robot.hpp"


using namespace fasio::logging;

void C2BSession::onClose()
{
    unenableRetry();
}

void C2BSession::sendInitData()
{
    //向 balance发送连接请求。 返回合适的GateServer配置信息
    CPGClientServer::ConnectRQ rq;
    rq.set_logicid(uuid());
    SessionManager.sendMsgToSession(shared_from_this(), rq, kClientConnectRQ, ServerType_BalanceServer);
}


bool C2BSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kClientConnectRS:
        {
            connectRS(buffer, header.size);
            break;
        }
        default:
        {
            LOG_ERROR << " error msg :" << header.type;
            break;
        }
    }
    return true;
}

void C2BSession::connectRS(const void* data, int len)
{
    LOG_MINFO << " ";
    CPGClientServer::ConnectRS rs;
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
        LOG_ERROR << " not parse data to CPGClientServer::ConnectRS size: " << len;
    }
}

///////////////////////////////////////////////////

void RobotSession::sendInitData()
{
    // 登录请求
    CPGClientServer::LoginRQ rq;
    rq.set_uid(uuid());
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "Robot-%d",uuid());
    rq.set_token(buf);
    SessionManager.sendMsgToSession(shared_from_this(), rq, kClientLoginRQ,ServerType_GateServer);
    updateHeartBeat();
}

void RobotSession::onClose() 
{
//    unenableRetry();
}


bool RobotSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    switch (header.type) {
        case kClientLoginRS:
        {
            gRobotManager.loginRS(session, buffer, header);
            break;
        }
        case kMatchListRS:
        {
            gRobotManager.matchListRS(session, buffer, header);
            break;
        }
        case kJoinMatchRS:
        {
            gRobotManager.joinMatchRS(session, buffer, header);
            break;
        }
        case kUnjoinMatchRS:
        {
            gRobotManager.unjoinMatchRS(session, buffer, header);
            break;
        }
        default:
            break;
    }
    return true;
}
 
