//
//  ServiceKernel.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/8.
//  Copyright © 2018年 fenggeren. All rights reserved.
//
#pragma once
#include <string>
#include <unordered_map>
#include "base/ServerNetConfig.h"
#include "Header.h"
namespace fasio
{
    class TCPSession;
    class TCPSessionFactory;
    class TCPSessionManager;
    
    struct ServiceConfig
    {
        short type;
        unsigned short port;
        short sid;
        std::string    ip;
        std::shared_ptr<TCPSession> session_;
    };
    
    class ServiceKernel
    {
    public:
        
        int checkConnectService(const ServiceConfig& config);
        
        void addNewConnect(short type,
                           unsigned short port,
                           short sid,
                           const std::string& ip);
        
        // @sid  service id
        std::shared_ptr<TCPSession> connectServiceSession(int sid) const;
        std::shared_ptr<TCPSession> connectSession(int uuid) const;
    protected:
        
        void netInitializer(const ServerNetConfig::ServerInfo& info,
                            asio::io_context& ioc,
                            TCPSessionManager& manager);
        
        virtual std::shared_ptr<TCPSession>
        connectService(unsigned short type,
                       unsigned short port,
                       short sid,
                       const std::string& ip) = 0;
        
        virtual
        std::shared_ptr<TCPSessionFactory>
        sessionFactory(int type, asio::io_context& ioc) = 0;
    protected:
        std::unordered_map<int, ServiceConfig> connectServices_;
    };

}



