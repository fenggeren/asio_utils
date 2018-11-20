//
//  G2MSession.cpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "G2MSession.hpp"
#include "GSKernel.hpp"

bool G2MSession::handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header)
{
    if (header.type >= GSTransToMSFromCP_Begin &&
        header.type <= GSTransToMSFromCP_End)
    {
        GSKernel::instance().transToClient(buffer, header);
    }
    return true;
}
void G2MSession::onClose()
{
    unenableRetry();
    GSKernel::instance().removeServiceSession(uuid());
}
 
