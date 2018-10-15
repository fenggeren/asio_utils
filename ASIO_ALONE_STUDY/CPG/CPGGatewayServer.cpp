//
//  CPGGatewayServer.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "CPGGatewayServer.hpp"
#include "Conv.hpp"

using namespace std::placeholders;
CPGGatewayServer::CPGGatewayServer()
{
    server_ = std::make_shared<TCPServer>(getIoContext(), 5578);
    server_->setMessageCallback(std::bind(&CPGGatewayServer::messageCallback, this, _1, _2));
    server_->setConnectionCallback(std::bind(&CPGGatewayServer::connectionCallback, this, _1));
    server_->setRemoveConnectionCallback(std::bind(&CPGGatewayServer::connectionCallback, this, _1));
}

void CPGGatewayServer::messageCallback(const std::shared_ptr<TCPSession>&, DataBuffer*const)
{
    
}

void CPGGatewayServer::connectionCallback(const std::shared_ptr<TCPSession>&)
{
    
}

void CPGGatewayServer::removeConnection(const std::shared_ptr<TCPSession>&)
{
    
}
