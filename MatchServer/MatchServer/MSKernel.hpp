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
    
    void transToCS(const void* data, const PacketHeader& header);
    
private:
    
    void runOneService(const ServerNetConfig::ServerInfo& config);
    
public:
    
    void checkMatchDistributeRS(const void* buffer, const PacketHeader& header);
    void distibuteMatchesNotify(const void* buffer, const PacketHeader& header);
protected:
    
    virtual std::shared_ptr<TCPSession>
    connectService(unsigned short type,
                   unsigned short port,
                   unsigned short sid,
                   const std::string& ip) override;
    virtual
    std::shared_ptr<TCPSessionFactory>
    sessionFactory(int type, asio::io_context& ioc) override;
public: 
    
    std::shared_ptr<TCPSession> centralSession_;
};

