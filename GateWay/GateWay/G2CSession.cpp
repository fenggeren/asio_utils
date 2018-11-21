//
//  G2CSession.cpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "G2CSession.hpp"
#include <CPG/CPGServer.pb.h>
#include <CPG/CPGServerDefine.h>
#include "GSSessionManager.hpp"
#include "GSKernel.hpp"
#include <Net/Util/NetPacket.hpp>
#include <Net/Util/ParseProto.hpp>

using namespace fasio::logging;

void G2CSession::onClose()
{
//    GSKernel::instance().removeServiceSession(uuid());
}

void G2CSession::sendInitData()
{
    sendRegisterData(SessionManager);
}


bool G2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    // 消息转发，不做解析处理
    if (header.type >= GSTransToCSFromCP_Begin &&
        header.type <= GSTransToCSFromCP_End)
    {
         GSKernel::instance().transToClient(buffer, header);
    }
    else
    {
        switch (header.type) {
            case kServerAllMatchDistributeNotify:
            {
                GSKernel::instance().distibuteMatchesNotify(buffer, header);
                break;
            }
            default:
                break;
        }
    }
    return true;
}
ServiceKernel& G2CSession::serviceKernel()
{
    return GSKernel::instance();
}

