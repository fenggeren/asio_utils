//
//  ServerConfigManager.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <vector>
#include <string>
#include "../../Net/base/ServerNetConfig.h"


class ServerConfigManager
{
public:
    
    static ServerConfigManager& instance()
    {
        static ServerConfigManager manager;
        return manager;
    }
    
    ServerConfigManager();
    
    bool configForType(int type, ServerNetConfig& config);
    // 获取给定 监听类型对应的 信息.
    
    bool listenInfo(const ServerNetConfig::ServerInfo& info,
                    int type,
                    ServerNetConfig::ServerNetInfo& listen);
    
    const ServerNetConfig& config() const
    {
        return config_;
    }
 
    int type() const { return type_; }
    void setType(int type);
    
    static std::string uuid(const ServerNetConfig::ServerInfo& info);
    
private:
    
    
    void load();
    
    // 解析所有连接信息
    void parseConnectors();
    
    std::vector<ServerNetConfig::ServerNetInfo>
    connectInfos(int connectType, int thisType);
    
private: 
    std::vector<ServerNetConfig> services_;
    ServerNetConfig config_;
    int type_;
};







