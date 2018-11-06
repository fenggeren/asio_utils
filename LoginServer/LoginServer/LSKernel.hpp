//
//  LSKernel.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/CPGHeader.h>
using namespace fasio;
static asio::io_context g_IoContext;

class LSKernel
{
public:
    static LSKernel& instance()
    {
        static LSKernel kernel;
        return kernel;
    }
    
    void start();
public:
    void addNewConnect(int type, int port, int serverid, const std::string& ip);
    
private:
    
};

