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
     
    
    void transToCS(const void* data, const PacketHeader& header);
    void transToClient(const void* data, const PacketHeader& header);
private:
    void runOneService(const ServerNetConfig::ServerInfo& config);
    
protected:
    
    virtual std::shared_ptr<TCPSession>
    connectService(unsigned short type,
                   unsigned short port,
                   int sid,
                   const std::string& ip) override;
    virtual
    std::shared_ptr<TCPSessionFactory>
    sessionFactory(int type, asio::io_context& ioc) override;
    virtual void updateServiceConnect(std::shared_ptr<TCPSession> session, State) override;
private:
    TCPSessionPtr centralSession_;
};

