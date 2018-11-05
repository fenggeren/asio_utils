//
//  CPGServerDefine.h
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <string>



#define kServerRegistRQ     20001
#define kServerRegistRS     20002
#define kServerLoginRQ      20003
#define kServerLoginRS      20004











enum ServerType
{
    ServerType_Gate_Login = 0,
    ServerType_Gate_Match,
    ServerType_Gate_Central,
    ServerType_Match_Central,
    ServerType_Login_Central,
    ServerType_GateServer,
    ServerType_LoginServer,
    ServerType_MatchServer,
    ServerType_CentralServer,
    ServerType_Client
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
    unsigned char           type{0};
    unsigned short          port{0};
    unsigned int            sid{0};
    unsigned int            connectTimes{0};
    unsigned int            lastHeartTimes{0};
    unsigned long long      msgReceived{0};
    unsigned long long      msgSended{0};
    unsigned long long      dataReceived{0};
    unsigned long long      dataSended{0};
    unsigned long long      loaded{0};  // 负载
    std::string             ip;
    std::string    exportIp;      // 对外ip  只有GateWay有效，其他服务是无效的！
};





















