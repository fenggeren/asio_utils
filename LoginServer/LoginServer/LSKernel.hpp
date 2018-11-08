//
//  LSKernel.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/CPGHeader.h>
#include <Net/ServiceKernel.hpp>
using namespace fasio;
static asio::io_context g_IoContext;

class LSKernel : public ServiceKernel
{
public:
    static LSKernel& instance()
    {
        static LSKernel kernel;
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
    
private:
    std::shared_ptr<TCPSession> centralSession_;
};


