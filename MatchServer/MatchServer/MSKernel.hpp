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
#include <google/protobuf/message.h>

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
    void runOneService(const ServerNetConfig::ServerInfo& config);
    void transToCS(const void* data, const PacketHeader& header);
    void sendMsg(const std::shared_ptr<TCPSession>& session,
                 google::protobuf::Message& msg,
                 int type, int extraID);
public:
    
    void checkMatchDistributeRS(const void* buffer, const PacketHeader& header);
    void distibuteMatchesNotify(const void* buffer, const PacketHeader& header);
    
    
    // gs => ms from client
    void joinMatchRQ(const TCPSessionPtr& session,
                     const void* buffer,
                     const PacketHeader& header);
    void unjoinMatchRQ(const TCPSessionPtr& session,
                       const void* buffer,
                       const PacketHeader& header);
    
    
    
    
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
public: 
    
    std::shared_ptr<TCPSession> centralSession_;
};

