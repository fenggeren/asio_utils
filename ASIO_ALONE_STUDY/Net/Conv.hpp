//
//  Conv.hpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/10/9.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include "Header.h"

namespace fasio
{
    using Handler = std::function<void()>;
    using CompleteHandler = std::function<void()>;
    
    asio::io_context& getIoContext();
    asio::io_context& getIoContext(const pthread_t& id);
 
    asio::io_context& getIoContext(const std::string& name);
    void setCurThreadName(const std::string& name);
    std::string getThreadName(const pthread_t& pid);
    
    void dispatchAfter(double delay, Handler&& handler);
    void dispatchAsync(Handler&& handler, asio::io_context& io);
    
    
    
}




