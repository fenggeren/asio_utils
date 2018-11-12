//
//  Conv.cpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/10/9.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "Conv.hpp"
#include "Queue.hpp"

namespace fasio
{
    
#define ioContexts ioContextMap()
#define nameThreads nameThreadMap()

    std::map<std::__thread_id, asio::io_context>&
    ioContextMap()
    {
        static std::map<std::__thread_id, asio::io_context>* map = new std::map<std::__thread_id, asio::io_context>;
        return *map;
    }

    std::map<std::string, std::__thread_id>&
    nameThreadMap()
    {
        static std::map<std::string, std::__thread_id>* map = new std::map<std::string, std::__thread_id>;
        return *map;
    }

    asio::io_context& getIoContext()
    {
        //        auto iter = ioContexts.find(std::this_thread::get_id());
        //        if (iter == ioContexts.end()) {
        //        }
        return ioContexts[std::this_thread::get_id()];
    }

    asio::io_context& getIoContext(const std::string& name)
    {
        return ioContexts[std::this_thread::get_id()];
    }

    void ioContextLoopForever()
    {
        auto guard = asio::make_work_guard(getIoContext());
        getIoContext().run();
    }
    
    void dispatchAfter(double delay, Handler&& handler)
    {
        TimerManager::createTimer(std::forward<Handler>(handler), getIoContext(), delay);
    }
    
    void dispatchAsync(Handler&& handler, asio::io_context& io)
    {
        asio::dispatch(io, std::forward<Handler>(handler));
    }

    
    void foreverQueue()
    {
        using namespace queue;
        Queue queue;
        std::thread thread([&]{
            queue.runForever();
        });
        thread.detach();
    }
    
}
