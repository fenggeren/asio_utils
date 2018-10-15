//
//  CPGGatewayServer.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include "TCPServer.hpp"
using namespace fasio;

class CPGGatewayServer
{
public:
    
    CPGGatewayServer();
    
    void start();
    
private:
    
    void messageCallback(const std::shared_ptr<TCPSession>&, DataBuffer*const);
    void connectionCallback(const std::shared_ptr<TCPSession>&);
    void removeConnection(const std::shared_ptr<TCPSession>&);
    
private:
    std::shared_ptr<TCPServer> server_;
};

