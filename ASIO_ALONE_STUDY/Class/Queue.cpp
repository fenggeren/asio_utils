//
//  Queue.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/16.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "Queue.hpp"

namespace Queue
{
#define ioContexts ioContextMap()
    
    std::map<std::__thread_id, asio::io_context>&
    ioContextMap()
    {
        static std::map<std::__thread_id, asio::io_context>* map = new std::map<std::__thread_id, asio::io_context>;
        return *map;
    }
    
    asio::io_context& getIoContext()
    {
//        auto iter = ioContexts.find(std::this_thread::get_id());
//        if (iter == ioContexts.end()) {
//        }
        return ioContexts[std::this_thread::get_id()];
    }
    
    void ioContextLoopForever()
    {
        auto guard = asio::make_work_guard(getIoContext());
        getIoContext().run();
    }
    
    void dispatchAfter(double delay, Handler&& handler)
    {
        TimerManager::createTimer(delay, std::forward<Handler>(handler), getIoContext());
    }
    
}





