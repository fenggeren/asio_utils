//
//  ServerConfigManager.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "ServerConfigManager.hpp"
#include "../Third/json/json.hpp"
#include <fstream>

using namespace nlohmann;


ServerConfigManager::ServerConfigManager()
{
    load();
    parseConnectors();

}

void ServerConfigManager::setType(int type)
{
    type_ = type;
    
    if(configForType(type_, config_) == false)
    {
        assert(0);
    }
}

ServerNetConfig::ServerNetInfo convertListen(json& listen)
{
    ServerNetConfig::ServerNetInfo linfo;
    linfo.type = listen["type"].get<int>();
    linfo.port = listen["port"].get<unsigned short>();
    return linfo;
}

ServerNetConfig::ServerInfo convertInfo(json& info)
{
    ServerNetConfig::ServerInfo sinfo;
    sinfo.ip = info["ip"].get<std::string>();
    std::vector<json> listens = info["listen"];
    for(auto& listen : listens)
    {
        ServerNetConfig::ServerNetInfo linfo;
        linfo = convertListen(listen);
        linfo.ip = sinfo.ip;
        sinfo.listenInfos.push_back(linfo);
    }
    
    auto connectJson = info["connect"];
    if (connectJson.is_null() == false)
    {
        std::vector<int> connects = connectJson;
        sinfo.connectTypes = connects;
    }
    
    return sinfo;
}

void ServerConfigManager::load()
{
    std::ifstream in("ServerConfig.json");
    nlohmann::json file_json;
    in >> file_json;
    
    if (file_json.is_null())
    {
        printf("match.config.json file is invalid!!!!!");
        assert(0);
        return;
    }
    
    services_.clear();
    std::vector<json> services = file_json["data"];
    for(auto& service : services)
    {
        ServerNetConfig config;
        config.name = service["name"].get<std::string>();
        config.type = service["type"].get<int>();
        
        std::vector<json> infos = service["info"];
        for(auto& info : infos)
        {
            ServerNetConfig::ServerInfo  sinfo = convertInfo(info);
            config.infos.push_back(sinfo);
        }
        
        services_.push_back(config);
    }
}



bool
ServerConfigManager::configForType(int type, ServerNetConfig& config)
{
    for(auto& service : services_)
    {
        if (service.type == type)
        {
            config = service;
            return true;
        }
    }
    printf("Error: not this type %d", type);
    return false;
}

bool
ServerConfigManager::listenInfo(const ServerNetConfig::ServerInfo& info,
                int type,
                ServerNetConfig::ServerNetInfo& olisten)
{
    for (auto& listen : info.listenInfos)
    {
        if (listen.type == type)
        {
            olisten = listen;
            return true;
        }
    }
    printf("Error: not this type %d in info", type);
    return false;
}


std::vector<ServerNetConfig::ServerNetInfo>
ServerConfigManager::connectInfos(int connectType, int thisType)
{
    std::vector<ServerNetConfig::ServerNetInfo> connects;
    
    //通过连接类型 ， 获取被连接服务的 信息
    ServerNetConfig config;
    if (configForType(connectType, config))
    {
        for(auto& info : config.infos)
        {
            ServerNetConfig::ServerNetInfo iinfo;
            // 通过连接服务的类型，获取 被连接服务的监听信息
            if (listenInfo(info, thisType, iinfo))
            {
                // 这里是 被连接服务的类型 而不是 type的类型
                iinfo.type = config.type;
                connects.push_back(iinfo);
            }
        }
    }

    
    return connects;
}

void ServerConfigManager::parseConnectors()
{
    for(auto& service : services_)
    {
        for(auto& info : service.infos)
        {
            std::vector<int> types = info.connectTypes;
            // 通过json的连接类型， 获取连接信息
            for(auto type : types)
            {
                for (auto& conn : connectInfos(type, service.type))
                {
                    info.connectInfos.push_back(conn);
                }
            }
        }
    }
}

std::string ServerConfigManager::uuid(const ServerNetConfig::ServerInfo& info)
{
    return info.ip;
}




