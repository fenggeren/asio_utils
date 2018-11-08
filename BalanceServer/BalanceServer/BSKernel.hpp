//
//  BSKernel.hpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <unordered_map>
#include <Net/TCPSession.hpp>
#include <Net/ServiceKernel.hpp>
#include <CPG/CPGHeader.h>
#include <google/protobuf/message.h>

using namespace fasio;
static asio::io_context g_IoContext;

class B2CSession;

class BSKernel : public ServiceKernel
{
public:
    
    static BSKernel& instance()
    {
        static BSKernel kernel;
        return kernel;
    }
    
    void start();
    
    void removeConnectService(int uuid);
    
    void transToCS(const void* data, const PacketHeader& header);
protected:
    
    virtual std::shared_ptr<TCPSession>
    connectService(unsigned short type,
                   unsigned short port,
                   unsigned short sid,
                   const std::string& ip) override;
    
private:
    TCPSessionPtr centralSession_;
};

