//
//  WSKernel.hpp
//  WebServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//
#pragma once
#include <Net/ServiceKernel.hpp>

using namespace fasio;

class WSKernel : public ServiceKernel
{
public:
    static WSKernel& instance()
    {
        static WSKernel kernel;
        return kernel;
    }
};
