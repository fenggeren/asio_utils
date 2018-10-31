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

namespace fasio
{

void TCPSessionManager::createListener(int port, bool ipv6, std::shared_ptr<TCPSessionFactory> factory)
{
    std::shared_ptr<TCPListener> listener(new TCPListener(factory));
    listener->setNewSessionCallback(std::bind(&TCPSessionManager::newSession, this, std::placeholders::_1));
    listener->listen(port, ipv6);
    listeners_.insert({port, listener});
}

void TCPSessionManager::newSession(std::shared_ptr<TCPSession> session)
{
    sessionMap_[session->uuid()] = session;
}

}

