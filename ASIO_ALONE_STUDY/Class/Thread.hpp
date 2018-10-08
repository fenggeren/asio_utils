//
//  Thread.hpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/10/8.
//  Copyright © 2018年 fenggeren. All rights reserved.
//


#pragma once

#include <thread>
#include <string>

namespace fasio
{ 
    static void setThreadName(std::string&& name);
    static const std::string& getThreadName();
}



