//
//  W2CSession.cpp
//  WebServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "W2CSession.hpp"
#include <CPG/CPGServer.pb.h>
#include <CPG/CPGServerDefine.h>
#include "WSKernel.hpp"
#include <Net/FASIO.hpp>

using namespace fasio::logging;

void W2CSession::onClose()
{
    //    GSKernel::instance().removeServiceSession(uuid());
}

void W2CSession::sendInitData()
{ 
}


bool W2CSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
 
    return true;
}
ServiceKernel& W2CSession::serviceKernel()
{
    return WSKernel::instance();
}
