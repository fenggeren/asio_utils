//
//  GSKernel.cpp
//  GateWay
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "GSKernel.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <Net/Util/NetPacket.hpp>
#include "CPGServerDefine.h"

using namespace fasio::logging;

void GSKernel::serverRegistRS(TCPSessionPtr session,
                              const void* data, int len)
{
    CPGToCentral::ServerRegisterRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
//        SessionManager.getSession(<#int32 uuid#>)
        if (rs.result() == 0)
        {
            
        }
        else
        {
            LOG_ERROR << " gs regist failure result: " << rs.result();
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << session->uuid();
    }
}
void GSKernel::serverLoginRS(TCPSessionPtr session,
                             const void* data, int len)
{
    
}

void GSKernel::addNewConnect(int type, int port, int serverid,
                             const std::string& ip)
{
    if(SessionManager.getClientSession(serverid))
    {
        // 已经连接到 该服务. 添加连接状态？！
        LOG_MINFO << " has connect service: type" << type
        << " port: " << port
        << " ip: " << ip
        << " serverid: " << serverid;
    }
    else
    {
        auto clientSession =  SessionManager.createConnector(type, g_IoContext, ip, port);
        clientSession->setLogicID(serverid);
    }
}




