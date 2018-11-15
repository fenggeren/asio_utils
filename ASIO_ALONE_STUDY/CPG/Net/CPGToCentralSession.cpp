//
//  CPGToCentralSession.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CPGToCentralSession.hpp"
#include "../../Net/Util/NetPacket.hpp"
#include "../../Net/Util/ParseProto.hpp"
#include "../../Net/logging/Logging.hpp"
#include "../../Net/ServiceKernel.hpp"
#include "CPGServerDefine.h"
#include <CPG/CPGToCentral.pb.h>
#include <list>
#include "../Util/ServerConfigManager.hpp"
#include "../../Net/TCPSessionManager.hpp"

using namespace logging;

void CPGToCentralSession::defaultMessageCallback(
                                              const std::shared_ptr<TCPSession>& session,
                                              DataBuffer*const data)
{
    while (hasPacket(data->peek(), data->readableBytes()))
    {
        PacketHeader* header = (PacketHeader*)data->peek();
        const void* buffer = data->peek() + kPacketHeaderSize;
        switch (header->type)
        {
            case kServerRegistRS:
            {
                serverRegistRS(buffer, header->size);
                break;
            }
            case kServerNewServicesNotify:
            {
                newServicesNotify(buffer, header->size);
                break;
            }
            default:
            {
                handlerMsg(session, buffer, *header);
                break;
            }
        }
        data->retrieve(kPacketHeaderSize + header->size);
    }
}

void CPGToCentralSession::newServicesNotify(const void* data, int len)
{
    CPGToCentral::NewConnServiceNotify rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        for(auto& connsvr : rs.connservers())
        {
            serviceKernel().addNewConnect(connsvr.type(), connsvr.port(), connsvr.sid(), connsvr.ip());
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}
void CPGToCentralSession::serverRegistRS(const void* data, int len)
{
    CPGToCentral::ServerRegisterRS rs;
    if (fasio::parseProtoMsg(data, len, rs))
    {
        if (rs.result() == 0)
        {
            setLogicID(rs.sid()); // 设置server id
            LOG_MINFO << " server id: " << rs.sid();
            
            for(auto& connsvr : rs.connservers())
            {
                serviceKernel().addNewConnect(connsvr.type(), connsvr.port(), connsvr.sid(), connsvr.ip());
            }
        }
        else
        {
            LOG_ERROR << " gs regist failure result: " << rs.result();
        }
    }
    else
    {
        LOG_ERROR << " cant parse proto msg len: " << len
        << " sessionID: " << uuid();
    }
}

std::list<ServerNetConfig::ServerNetInfo>
listenInfos()
{
    std::list<ServerNetConfig::ServerNetInfo> listens;
    const auto& config = ServerConfigManager::instance().config();
    for(auto& info : config.infos)
    {
        for(auto& listen : info.listenInfos)
        {
            listens.push_back(listen);
        }
    }
    
    listens.unique([](const ServerNetConfig::ServerNetInfo& lv,
                      const ServerNetConfig::ServerNetInfo& rv)
                   {
                       return lv.type == rv.type &&
                       lv.ip == rv.ip &&
                       lv.port == rv.port;
                   });
    
    return listens;
}

void CPGToCentralSession::sendRegisterData(TCPSessionManager& sessionManager)
{
    auto listens = listenInfos();
    for(auto& listen : listens )
    {
        CPGToCentral::ServerRegisterRQ rq;
        rq.set_type(listen.type);
        rq.set_port(listen.port);
        rq.set_sid(logicID());
        rq.set_ip(listen.ip);
        rq.set_exportip(listen.ip);
        
        sessionManager.sendMsgToSession(shared_from_this(), rq,
                                        kServerRegistRQ, ServerType_CentralServer);
    }
}
