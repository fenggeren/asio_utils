//
//  GSKernel.hpp
//  GateWay
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <unordered_map>
#include <Net/TCPSession.hpp>
#include <Net/ServiceKernel.hpp>
#include <CPG/CPGHeader.h>
#include <CPG/CPGServerDefine.h>
#include <google/protobuf/message.h>
#include <list>

using namespace fasio;
static asio::io_context g_IoContext;


class GSKernel : public ServiceKernel
{
public:
    
    static GSKernel& instance()
    {
        static GSKernel kernel;
        return kernel;
    }
    
    void start();
    
    void removeConnectService(int uuid);
    
private:
     void runOneService(const ServerNetConfig::ServerInfo& config);
    
public:
    
    void transToLS(const void* data, const PacketHeader& header);
    void transToCS(const void* data, const PacketHeader& header);
    void transToMS(const void* data, const PacketHeader& header, int mid);
protected:
    
    virtual std::shared_ptr<TCPSession>
    connectService(unsigned short type,
                   unsigned short port,
                   unsigned short sid,
                   const std::string& ip) override;
    virtual
    std::shared_ptr<TCPSessionFactory>
    sessionFactory(int type, asio::io_context& ioc) override;
private:
    // 比赛id <=> match info
    std::unordered_map<uint32, TCPSessionPtr> matchesServices_;
    
    TCPSessionPtr loginSession_;
    TCPSessionPtr centralSession_;
};





