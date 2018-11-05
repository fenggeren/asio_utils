//
//  MSKernel.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once 
#include <CPG/CPGHeader.h>


using namespace fasio;
static asio::io_context g_IoContext;

class MSKernel
{
public:
    static MSKernel& instance()
    {
        static MSKernel kernel;
        return kernel;
    }
    
    void start();
    
public:
    void addNewConnect(int type, int port, int serverid, const std::string& ip);
    
     
};

