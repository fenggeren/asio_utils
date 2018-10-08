//
//  Thread.cpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/10/8.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "Thread.hpp"
#include <map>
#include <string>

namespace fasio
{
    
    static thread_local std::string threadName;
    
    
    using nameThreadMap = std::map<std::string, std::__thread_id>;
    
    nameThreadMap&
    ioContextMap()
    {
        static nameThreadMap* map = new nameThreadMap;
        return *map;
    }
    
    static void setThreadName(std::string&& name)
    {
        threadName = std::move(name);
    }
    static const std::string& getThreadName()
    {
        return threadName;
    }
}


