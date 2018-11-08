//
//  ServiceKernel.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/8.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "ServiceKernel.hpp"
#include "TCPSession.hpp"
#include "logging/Logging.hpp"

namespace fasio
{
    using namespace logging;
    
    bool operator==(const ServiceConfig& sc1, const ServiceConfig& sc2)
    {
        return sc1.type == sc2.type &&
                sc1.port == sc2.port &&
                sc1.ip == sc2.ip;
    }
    
    int ServiceKernel::checkConnectService(const ServiceConfig& config)
    {
        for(auto& pair : connectServices_)
        {
            if (pair.second == config) {
                return pair.first;
            }
        }
        return INVALID_UUID;
    }
    
    void ServiceKernel::addNewConnect(unsigned short type,
                       unsigned short port,
                       unsigned short sid,
                       const std::string& ip)
    {
        ServiceConfig config{type, port, sid, ip};
        int uuid = checkConnectService(config);
        if (uuid == INVALID_UUID)
        {
            // 正常创建新连接
            auto session = connectService(type, port, sid, ip);
            config.session_ = session;
            connectServices_[session->uuid()] = config;
        }
        else
        {
            // 旧服务-- 更新sid， 已连接的service A重启后， A从新配置sid， 这里更新--
            auto& oldConfig = connectServices_[uuid];
            
            LOG_MINFO << " has connect service's session, there sid must update "
            << " oldsid: " << oldConfig.sid
            << " newsid: " << config.sid;
            
            oldConfig.sid = config.sid;
            oldConfig.session_->setLogicID(config.sid);
        }
    }
}







