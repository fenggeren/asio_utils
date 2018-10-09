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
#include "Header.h"

namespace fasio
{ 
    static void setThreadName(std::string&& name);
    static const std::string& getThreadName();
    
    struct ThreadInfo
    {
        ThreadInfo(const std::__thread_id& tid, const std::string& name)
        : tid_(tid), name_(name){}
        const std::__thread_id tid_;
        const std::string name_;
        const asio::io_context ictx_;
    };
    
}



