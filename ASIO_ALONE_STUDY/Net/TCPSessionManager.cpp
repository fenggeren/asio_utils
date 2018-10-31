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
    
void TCPSessionManager::createConnector(uint8 type, asio::io_context& io,
                                        const std::string& ip, uint16 port)
{
    std::shared_ptr<TCPConnector> connector(new TCPConnector(io, ip, port));
    auto session = createConnectorSession(type);
    addSession(session);
    session->setConnector(connector);
    connector->start();
}
    
std::shared_ptr<ClientSession>
    TCPSessionManager::createConnectorSession(uint8 type)
{
    return std::make_shared<ClientSession>();
}

void TCPSessionManager::newSession(std::shared_ptr<TCPSession> session)
{
    LOG_MINFO << session->uuid();
    
    addSession(session);
}

void TCPSessionManager::removeSessionPtr(const TCPSessionPtr& session)
{
    removeSession(session->uuid());
}
void TCPSessionManager::removeSession(int32 uuid)
{
    sessionMap_.erase(uuid);
}
void TCPSessionManager::addSession(TCPSessionPtr session)
{
    if (sessionMap_.find(session->uuid()) != sessionMap_.end())
    {
        LOG_MINFO << " session had exist: " << session->uuid();
    }
    sessionMap_[session->uuid()] = session;
    session->setCloseCallback(std::bind(&TCPSessionManager::removeSessionPtr, this, std::placeholders::_1));
}

inline TCPSessionPtr TCPSessionManager::getSession(int32 uuid)
{
    auto iter = sessionMap_.find(uuid);
    if (iter != sessionMap_.end()) {
        return iter->second;
    }
    return nullptr;
}

void TCPSessionManager::sendMsgToSession(int32 uuid, const void* data, int len, int msgID, uint8 stype)
{
    PacketHeader header{msgID, len};
    if (uuid > 0)
    {
        auto iter = sessionMap_.find(uuid);
        if (iter != sessionMap_.end())
        {
            iter->second->addMore(&header, sizeof(PacketHeader));
            iter->second->send(data, len);
        }
    }
    else
    {
        for (auto& pair : sessionMap_)
        {
            if (pair.second->type() == stype)
            {
                pair.second->addMore(&header, sizeof(PacketHeader));
                pair.second->send(data, len);
            }
        }
    }
}
    
}



