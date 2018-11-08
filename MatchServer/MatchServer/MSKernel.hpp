//
//  MSKernel.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once 
#include <CPG/CPGHeader.h>
#include <Net/ServiceKernel.hpp>

using namespace fasio;
static asio::io_context g_IoContext;

class MSKernel : public ServiceKernel
{
public:
    static MSKernel& instance()
    {
        static MSKernel kernel;
        return kernel;
    }
    
    void start();
    
    void removeConnectService(int uuid);
    
protected:
    
    virtual std::shared_ptr<TCPSession>
    connectService(unsigned short type,
                   unsigned short port,
                   unsigned short sid,
                   const std::string& ip) override;
    
public: 
    
    std::shared_ptr<TCPSession> centralSession_;
};

