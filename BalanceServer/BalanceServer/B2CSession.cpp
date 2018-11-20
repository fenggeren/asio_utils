//
//  B2CSession.cpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "B2CSession.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <CPG/CPGClient.pb.h>
#include <CPG/CPGServerDefine.h>
#include "BSSessionManager.hpp"
#include "BSKernel.hpp"
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>

using namespace fasio::logging;


void B2CSession::onClose()
{
    BSKernel::instance().removeServiceSession(uuid());
}

void B2CSession::sendInitData()
{
    sendRegisterData(SessionManager);
}

ServiceKernel& B2CSession::serviceKernel()
{
    return BSKernel::instance();
}


bool B2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                             const void* buffer, const PacketHeader& header)
{
    // 消息转发，不做解析处理
    if (header.type >= BSTransToCSFromCP_Begin &&
        header.type <= BSTransToCSFromCP_End)
    {
        BSKernel::instance().transToClient(buffer, header);
    }
    else
    {
        switch (header.type) {
            default:
                break;
        }
    }
    return true;
}


void B2CSession::connectRS(const void* data, const PacketHeader& header)
{
    LOG_MINFO << "";
    
    SessionManager.sendMsg(header.extraID, data, header);
}

