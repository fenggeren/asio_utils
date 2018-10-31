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

void TCPSessionManager::newSession(std::shared_ptr<TCPSession> session)
{
    LOG_MINFO << session->uuid();
    
    sessionMap_[session->uuid()] = session;
    session->setCloseCallback(std::bind(&TCPSessionManager::removeSessionPtr, this, std::placeholders::_1));
}

void TCPSessionManager::removeSessionPtr(const TCPSessionPtr& session)
{
    removeSession(session->uuid());
}
void TCPSessionManager::removeSession(int32 uuid)
{
    sessionMap_.erase(uuid);
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



