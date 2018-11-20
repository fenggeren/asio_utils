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
#include "TCPSessionManager.hpp"

namespace fasio
{
    using namespace logging;
    
    bool operator==(const ServiceConfig& sc1, const ServiceConfig& sc2)
    {
        return sc1.type == sc2.type &&
                sc1.port == sc2.port &&
                sc1.ip == sc2.ip;
    }
    
    void ServiceKernel::netInitializer(const ServerNetConfig::ServerInfo& info,
                                       asio::io_context& ioc,
                                       TCPSessionManager& manager)
    {
        for(auto& listen: info.listenInfos)
        {
            manager.createListener(listen.port, false,
                                          sessionFactory(listen.type, ioc));
        }
        
        for(auto& connect : info.connectInfos)
        {
            addNewConnect(connect.type, connect.port, INVALID_LID, connect.ip);
        }
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
    
    std::shared_ptr<TCPSession>
    ServiceKernel::connectServiceSession(int sid) const
    {
        for(auto& pair : connectServices_)
        {
            if (pair.second.sid == sid)
            {
                return pair.second.session_;
            }
        }
        return nullptr;
    }
    std::shared_ptr<TCPSession>
    ServiceKernel::connectSession(int uuid) const
    {
        auto iter = connectServices_.find(uuid);
        if (iter != connectServices_.end())
        {
            return iter->second.session_;
        }
        return nullptr;
    }
    
    void ServiceKernel::addNewConnect(short type,
                       unsigned short port,
                       int sid,
                       const std::string& ip)
    {
        LOG_MINFO << " type: " << type
        << " address: " << ip << ":" << port
        << " sid: " << sid;
        ServiceConfig config{type, port, sid, ip};
        int uuid = checkConnectService(config);
        // uuid: 动态返回的未连接该服务
        // sid: 启动后就发起的连接。
        if (uuid == INVALID_UUID || sid == INVALID_UUID)
        {
            // 正常创建新连接
            auto session = connectService(type, port, sid, ip);
            config.session_ = session;
            connectServices_[session->uuid()] = config;
            uuid = session->uuid();
            updateServiceConnect(session, kConnect);
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
            updateServiceConnect(oldConfig.session_, kUpdate);
        }
    }
    
    void ServiceKernel::removeServiceSession(int uuid)
    {
        auto iter = connectServices_.find(uuid);
        if (iter == connectServices_.end())
        {
            LOG_MINFO << " not found service  uuid: " << uuid;
        }
        else
        {
            updateServiceConnect(iter->second.session_,
                                 kClose);
            connectServices_.erase(iter);
        }
    }
}







