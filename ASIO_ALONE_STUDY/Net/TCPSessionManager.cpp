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
    addClientSession(session);
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
    LOG_MINFO << "sessionid: " << session->uuid()
            << " type: " << session->type() ;
    
    addSession(session);
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
            clientsession->reconnect();
        }
        else
        {
            removeSession(session->uuid());
            removeClientSession(session->uuid());
        }
    }
}
    
void TCPSessionManager::removeSession(int32 uuid)
{
    sessionMap_.erase(uuid);
}

void TCPSessionManager::removeClientSession(int32 logicid)
{
    auto iter = std::find_if(clientSessions_.begin(), clientSessions_.end(),
                             [logicid](const TCPSessionPtr& session)
              {
                  return session->logicID() == logicid;
              });
    if (iter != clientSessions_.end())
    {
        clientSessions_.erase(iter);
    }
    else
    {
        LOG_ERROR << " can't found client session :" << logicid;
    }
}
TCPSessionPtr TCPSessionManager::getClientSession(int32 logicid)
{
    auto iter = std::find_if(clientSessions_.begin(), clientSessions_.end(),
                             [logicid](const TCPSessionPtr& session)
                          {
                              return session->logicID() == logicid;
                          });
    if (iter != clientSessions_.end())
    {
        return *iter;
    }
    else
    {
        LOG_ERROR << " can't found client session :" << logicid;
        return nullptr;
    }
}
void TCPSessionManager::addClientSession(TCPSessionPtr session)
{
    clientSessions_.push_back(session);
}

void TCPSessionManager::addSession(TCPSessionPtr session)
{
    if (sessionMap_.find(session->uuid()) != sessionMap_.end())
    {
        LOG_MINFO << " session has exist: " << session->uuid();
    }
    sessionMap_[session->uuid()] = session;
    session->setCloseCallback(std::bind(&TCPSessionManager::removeSessionPtr, this, std::placeholders::_1));
}

TCPSessionPtr TCPSessionManager::getSession(int32 uuid)
{
    auto iter = sessionMap_.find(uuid);
    if (iter != sessionMap_.end()) {
        return iter->second;
    }
    return nullptr;
}

void TCPSessionManager::sendMsgToSession(int32 uuid, const void* data, int len, int msgID, uint8 stype)
{
    TCPSessionPtr session = nullptr;
    auto iter = sessionMap_.find(uuid);
    if (iter != sessionMap_.end())
    {
        session = iter->second;
    }
    sendMsgToSession(session, data, len, msgID, stype);
}
 
void TCPSessionManager::sendMsgToSession(int32 uuid,
                                         google::protobuf::Message& msg,
                                         int msgID, uint8 stype)
{
    TCPSessionPtr session = nullptr;
    auto iter = sessionMap_.find(uuid);
    if (iter != sessionMap_.end())
    {
        session = iter->second;
    }
    sendMsgToSession(session, msg, msgID, stype);
}
void TCPSessionManager::sendMsgToSession(int32 uuid,
                                         const std::string& msg,
                                         int msgID, uint8 stype)
{
    TCPSessionPtr session = nullptr;
    auto iter = sessionMap_.find(uuid);
    if (iter != sessionMap_.end())
    {
        session = iter->second;
    }
    sendMsgToSession(session, msg, msgID, stype);
}
void TCPSessionManager::sendMsgToSession(int32 uuid,
                                         std::shared_ptr<NetPacket> packet,
                                         uint8 stype)
{
    TCPSessionPtr session = nullptr;
    auto iter = sessionMap_.find(uuid);
    if (iter != sessionMap_.end())
    {
        session = iter->second;
    }
    sendMsgToSession(session, packet, stype);
}
void TCPSessionManager::sendMsgToSession(TCPSessionPtr session,
                                         const void* data, int len,
                                         int msgID, uint8 stype)
{
    PacketHeader header{msgID, len};
    if (session)
    {
        session->addMore(&header, kPacketHeaderSize);
        session->send(data, len);
    }
    else
    {
        for (auto& pair : sessionMap_)
        {
            if (pair.second->type() == stype)
            {
                pair.second->addMore(&header, kPacketHeaderSize);
                pair.second->send(data, len);
            }
        }
    }
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
        for (auto& pair : sessionMap_)
        {
            if (pair.second->type() == stype)
            {
                pair.second->send(data, len);
            }
        }
    }
}
    
    
void TCPSessionManager::transMsgToSession(TCPSessionPtr session, const void* data,
                                          int len,  int msgID,  int32 extraID,
                                          uint8 stype)
{
    PacketHeader header{msgID, len, extraID};
    if (session)
    {
        session->addMore(&header, kPacketHeaderSize);
        session->send(data, len);
    }
    else
    {
        for (auto& pair : sessionMap_)
        {
            if (pair.second->type() == stype)
            {
                pair.second->addMore(&header, kPacketHeaderSize);
                pair.second->send(data, len);
            }
        }
    }
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
    
}



