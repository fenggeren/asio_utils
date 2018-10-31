//
//  CPGGatewayServer.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "CPGGatewayServer.hpp"
#include "Conv.hpp"
#include "DataBuffer.hpp"
#include "TCPSession.hpp"

using namespace std::placeholders;
CPGGatewayServer::CPGGatewayServer()
{
    server_ = std::make_shared<TCPServer>(getIoContext(), 7237);
//    server_->setMessageCallback(std::bind(&CPGGatewayServer::messageCallback, this, _1, _2));
    server_->setConnectionCallback(std::bind(&CPGGatewayServer::connectionCallback, this, _1));
    server_->setRemoveConnectionCallback(std::bind(&CPGGatewayServer::connectionCallback, this, _1));
}

void CPGGatewayServer::start()
{
    server_->start();
    getIoContext().run();
}

void CPGGatewayServer::messageCallback(const std::shared_ptr<TCPSession>& session, DataBuffer*const data)
{
    data->retrieveAll();
    session->forceClose();
}

void CPGGatewayServer::connectionCallback(const std::shared_ptr<TCPSession>&)
{
    
}

void CPGGatewayServer::removeConnection(const std::shared_ptr<TCPSession>&)
{
    
}
