//
//  CPGToCentralSession.cpp
//  LoginServer
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CPGToCentralSession.hpp"
#include "CPGServerDefine.h"
#include <CPG/CPGToCentral.pb.h>
#include "../Util/ServerConfigManager.hpp"
#include "../../Net/FASIO.hpp"
#include <list>
#include <set>

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

std::set<int> connectTypes()
{
    std::list<ServerNetConfig::ServerNetInfo> listens;
    const auto& config = ServerConfigManager::instance().config();
    std::set<int> conTypes;
    
    for(auto& info : config.infos)
    {
        for(auto type : info.connectTypes)
        {
            conTypes.insert(type);
        }
    }
    return conTypes;
}



void CPGToCentralSession::sendRegisterData(TCPSessionManager& sessionManager)
{
    CPGToCentral::ServerRegisterRQ rq;
    rq.set_sid(logicID()); // 可能会重复注册，根据logicID判断
    rq.set_type(ServerType_MatchServer);
    
    auto listens = listenInfos();
    for(auto& listen : listens )
    {
        auto lis = rq.add_listeners();
        lis->set_ip(listen.ip);
        lis->set_port(listen.port);
        lis->set_type(listen.type);
    }
    auto conTypes = connectTypes();
    for(auto type : conTypes)
    {
        rq.add_connecttypes(type);
    }
    sessionManager.sendMsgToSession(shared_from_this(), rq,
                                    kServerRegistRQ, ServerType_CentralServer);
}
