//
//  CPGServerDefine.h
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <string>
#include <vector>

#define MAIN "main"




enum ServerType
{
//    ServerType_Gate_Login = 0,
//    ServerType_Gate_Match,
//    ServerType_Gate_Central,
//    ServerType_Match_Central,
//    ServerType_Login_Central,
//    ServerType_Balance_Central,
//    ServerType_Client_Balance,
//    ServerType_Client_Gate,
    ServerType_Client = 0,
    ServerType_GateServer,
    ServerType_LoginServer,
    ServerType_MatchServer,
    ServerType_BalanceServer,
    ServerType_CentralServer,
};

// 动态启动新的服务,配置
// sid由CentralServer分配， 全局唯一.
// 用于根据sid查找对应的服务。
struct ServerConfig
{
    unsigned short type;
    unsigned short port;
    unsigned short sid;
    std::string    ip;
    
    bool operator==(const ServerConfig& other)
    {
        return type == other.type &&
                port == other.port &&
                ip == other.ip;
    }
};

// GateServer是对外客户端的唯一服务， 需要区别内网/外网ip
struct GateServerConfig
{
    unsigned short type;
    unsigned short port;
    unsigned short sid;
    std::string    ip;            // 内网地址
    std::string    exportIp;      // 对外ip
};



// 服务器统计信息
struct ServerInfo
{
    // 服务器监听的信息.
    struct Listener
    {
        unsigned char           type{0};
        unsigned short          port{0};
        std::string             ip;
    };
    
    unsigned char           type{0};
    int                     sid{0};
    unsigned int            connectTimes{0};
    unsigned int            lastHeartTimes{0};
             int            loaded{0};  // 负载
    unsigned long long      msgReceived{0};
    unsigned long long      msgSended{0};
    unsigned long long      dataReceived{0};
    unsigned long long      dataSended{0};
    std::vector<Listener>     listeners; // 监听信息
    std::vector<int>         connectTypes; //需要连接其他的服务类型
};





















