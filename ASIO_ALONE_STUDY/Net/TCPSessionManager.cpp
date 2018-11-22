//
//  NetSessionManager.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/30.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPSessionManager.hpp"
#include "TCPSessionFactory.h"
#include "TCPListener.h"
#include "logging/Logging.hpp"
#include <functional>
#include "TCPConnector.hpp"
#include "Util/NetPacket.hpp"

namespace fasio
{
using namespace logging;
    
void TCPSessionManager::createListener(int port, bool ipv6, std::shared_ptr<TCPSessionFactory> factory)
{
    std::shared_ptr<TCPListener> listener(new TCPListener(factory));
    listener->setNewSessionCallback(std::bind(&TCPSessionManager::newSession, this, std::placeholders::_1));
    listener->listen(port, ipv6);
    listeners_.insert({port, listener});
}
    
std::shared_ptr<ClientSession> TCPSessionManager::createConnector(uint8 type, asio::io_context& io,
                                        const std::string& ip, uint16 port)
{
    std::shared_ptr<TCPConnector> connector(new TCPConnector(io, ip, port));
    auto session = createConnectorSession(type);
    session->setType(type);
    addSession(session);
    session->setConnector(connector);
    connector->start();
    return session;
}
    
std::shared_ptr<ClientSession>
    TCPSessionManager::createConnectorSession(uint8 type)
{
    return std::make_shared<ClientSession>();
}

void TCPSessionManager::newSession(std::shared_ptr<TCPSession> session)
{
 
    addSession(session);
    
    LOG_MINFO << "sessionid: " << session->uuid()
    << " type: " << session->type() ;
}
 
    
void TCPSessionManager::removeSessionPtr(const TCPSessionPtr& session)
{
    if (!session->isClient())
    {
        removeSession(session->uuid());
    }
    else
    {
        TCPSession* ssp = session.get();
        ClientSession* clientsession = dynamic_cast<ClientSession*>(ssp);
        if (clientsession && clientsession->canRetry())
        {
            LOG_DEBUG << " reconnect";
            clientsession->reconnect();
        }
        else
        {
            removeSession(session->uuid()); 
        }
    }
}
    
void TCPSessionManager::removeSession(int32 uuid)
{
//    sessionMap_.erase(uuid);
    sessionMap_.removeObject(uuid);
}

 

void TCPSessionManager::addSession(TCPSessionPtr session)
{
    int32 index = sessionMap_.addObject(session);
    session->setUUID(index);
    session->setCloseCallback(std::bind(&TCPSessionManager::removeSessionPtr, this, std::placeholders::_1));
}

TCPSessionPtr TCPSessionManager::getSession(int32 uuid)
{
    return (uuid >= 0 ? sessionMap_.getObject(uuid) : nullptr);
}

void TCPSessionManager::sendMsgToSession(int32 uuid, const void* data, int len, int msgID, uint8 stype)
{
    sendMsgToSession(getSession(uuid), data, len, msgID, stype);
}
 
void TCPSessionManager::sendMsgToSession(int32 uuid,
                                         google::protobuf::Message& msg,
                                         int msgID, uint8 stype)
{
    sendMsgToSession(getSession(uuid), msg, msgID, stype);
}
void TCPSessionManager::sendMsgToSession(int32 uuid,
                                         const std::string& msg,
                                         int msgID, uint8 stype)
{
    sendMsgToSession(getSession(uuid), msg, msgID, stype);
}
void TCPSessionManager::sendMsgToSession(int32 uuid,
                                         std::shared_ptr<NetPacket> packet,
                                         uint8 stype)
{
    sendMsgToSession(getSession(uuid), packet, stype);
}
void TCPSessionManager::sendMsgToSession(TCPSessionPtr session,
                                         const void* data, int len,
                                         int msgID, uint8 stype)
{
    PacketHeader header{msgID, len};
    sendMsg(session, data, header, stype);
}
void TCPSessionManager::sendMsgToSession(TCPSessionPtr session,
                                         google::protobuf::Message& msg,
                                         int msgID, uint8 stype)
{
    sendMsgToSession(session, msg.SerializeAsString(), msgID, stype);
}
void TCPSessionManager::sendMsgToSession(TCPSessionPtr session,
                                         const std::string& msg,
                                         int msgID, uint8 stype)
{
    sendMsgToSession(session, msg.data(), (int)msg.size(), msgID, stype);
}
void TCPSessionManager::sendMsgToSession(TCPSessionPtr session,
                                         std::shared_ptr<NetPacket> packet,
                                         uint8 stype )
{
    sendMsgToSession(session, packet->buffer(), packet->size(), stype);
}
    
void TCPSessionManager::sendMsgToSession(TCPSessionPtr session,
                                         const void* data,
                                         int len, uint8 stype)
{
    if (session)
    {
        session->send(data, len);
    }
    else
    {
        sessionMap_.foreach([&] (const std::shared_ptr<TCPSession> &session){
            if (session->type() == stype)
            {
                session->send(data, len);
            }
        });
    }
}
    
    
void TCPSessionManager::transMsgToSession(TCPSessionPtr session, const void* data,
                                          int len,  int msgID,  int32 extraID,
                                          uint8 stype)
{
    PacketHeader header{msgID, len, extraID};
    sendMsg(session, data, header, stype);
}
    
void TCPSessionManager::transMsgToSession(TCPSessionPtr session,
                                          const std::string& msg,
                                          int32 msgID, int32 extraID,
                                          uint8 stype)
{
    transMsgToSession(session, msg.data(), msg.size(), msgID, extraID, stype);
}
void TCPSessionManager::transMsgToSession(TCPSessionPtr session,
                                          google::protobuf::Message& msg,
                                          int32 msgID, int32 extraID,
                                          uint8 stype)
{
    transMsgToSession(session, msg.SerializeAsString(), msgID, extraID, stype);
}
    
    
void TCPSessionManager::sendMsg(TCPSessionPtr session, const void* body,
                                const PacketHeader& header, uint8 stype)
{
    if (session)
    {
        session->addMore(&header, kPacketHeaderSize);
        session->send(body, header.size);
    }
    else
    {
        sessionMap_.foreach([&] (const std::shared_ptr<TCPSession> &session){
            if (session->type() == stype)
            {
                session->addMore(&header, kPacketHeaderSize);
                session->send(body, header.size);
            }
        });
    }
}
    
void TCPSessionManager::sendMsg(int32 uuid, const void* body,
             const PacketHeader& header, uint8 stype)
{
    sendMsg(getSession(uuid), body, header, stype);
}
    
}



