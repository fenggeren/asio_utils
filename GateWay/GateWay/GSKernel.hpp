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
    
    
    
    void transToLS(const void* data, const PacketHeader& header);
    void transToCS(const void* data, const PacketHeader& header);
    void transToMS(const void* data, const PacketHeader& header, int mid);

    
public:
    // 比赛分配
    void distibuteMatchesNotify(const void* buffer, const PacketHeader& header);

private:
    void runOneService(const ServerNetConfig::ServerInfo& config);
    // 比赛id 对应的 matchService session
    TCPSessionPtr matchServiceSession(int mid);
    
protected:
    
    virtual std::shared_ptr<TCPSession>
    connectService(unsigned short type,
                   unsigned short port,
                   short sid,
                   const std::string& ip) override;
    virtual
    std::shared_ptr<TCPSessionFactory>
    sessionFactory(int type, asio::io_context& ioc) override;
private:
    // 比赛id, match service session
    std::unordered_map<int, TCPSessionPtr> matchesServices_;
    // sid , <mid>
    std::map<int, std::list<int>> matchesDistriMaps_; //
    
    TCPSessionPtr loginSession_;
    TCPSessionPtr centralSession_;
};





