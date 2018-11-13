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
    
    std::map<pthread_t, asio::io_context>&
    ioContextMap()
    {
        static auto* map = new std::map<pthread_t, asio::io_context>;
        return *map;
    }
    
    std::map<std::string, pthread_t>&
    nameThreadMap()
    {
        static auto* map = new std::map<std::string, pthread_t>;
        return *map;
    }

    asio::io_context& getIoContext()
    {
        return ioContexts[pthread_self()];
    }

    asio::io_context& getIoContext(const pthread_t& id)
    {
        auto iter = ioContexts.find(id);
        if (iter == ioContexts.end())
        {
            assert(0);
        }
        return ioContexts[id];
    }
    asio::io_context& getIoContext(const std::string& name)
    {
        auto iter = nameThreads.find(name);
        if (iter == nameThreads.end())
        {
            assert(0);
        }
        return ioContexts[iter->second];
    }
    
    void setCurThreadName(const std::string& name)
    {
        nameThreads[name] = pthread_self();
        pthread_setname_np(name.data());
    }
    std::string getThreadName(const pthread_t& pid)
    {
        char name[128] = {0};
        pthread_getname_np(pid, name, sizeof(name));
        return name;
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
