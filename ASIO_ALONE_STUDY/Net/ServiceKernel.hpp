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

namespace fasio
{
    class TCPSession;
    
    struct ServiceConfig
    {
        unsigned short type;
        unsigned short port;
        unsigned short sid;
        std::string    ip;
        std::shared_ptr<TCPSession> session_;
    };
    
    class ServiceKernel
    {
    public:
         
    public:
        
        int checkConnectService(const ServiceConfig& config);
        
        void addNewConnect(unsigned short type,
                           unsigned short port,
                           unsigned short sid,
                           const std::string& ip);
        
    protected:
        
        virtual std::shared_ptr<TCPSession>
        connectService(unsigned short type,
                       unsigned short port,
                       unsigned short sid,
                       const std::string& ip) = 0;
        
    protected:
        std::unordered_map<int, ServiceConfig> connectServices_;
    };

}



