//
//  BSKernel.hpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <unordered_map>
#include <Net/TCPSession.hpp>
#include <CPG/CPGHeader.h>
#include <google/protobuf/message.h>

using namespace fasio;
static asio::io_context g_IoContext;

class B2CSession;

class BSKernel
{
public:
    
    static BSKernel& instance()
    {
        static BSKernel kernel;
        return kernel;
    }
    
    void start();
    
public:
    void addNewConnect(int type, int port, int serverid, const std::string& ip);
    
public:
    
    void serverRegistRS(TCPSessionPtr session,
                        const void* data, int len);
private:
    
};

