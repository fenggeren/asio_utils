//
//  Queue.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/16.
//  Copyright © 2018年 fenggeren. All rights reserved.
//
#pragma once
#include <list>
#include <unordered_set>
#include "Header.h"
#include <iostream>
#include "Conv.hpp"
#include "TimerManager.hpp"

namespace fasio
{


namespace queue
{
 
#define MainQueue Queue::mainQueue()
    
    class Queue
    {
        using ExecutorGuard = asio::executor_work_guard<asio::io_context::executor_type>;
    public:
        
        static Queue& mainQueue()
        {
            static Queue queue;
            return queue;
        }
        
        Queue(asio::io_context& io = getIoContext())
        :io_context_(io)
        {
        }
        
        void run()
        {
            io_context_.run();
        }
        
        void runForever()
        {
            guard_ = std::make_shared<ExecutorGuard>(io_context_.get_executor());
            io_context_.run();
        }
        
        asio::io_context& ioContext()
        {
            return io_context_;
        }
        
        
        void post(Handler&& handler)
        {
            asio::post(io_context_, std::forward<Handler>(handler));
        }
        
        void dispatch(Handler&& handler)
        {
            asio::dispatch(io_context_, std::forward<Handler>(handler));
        }
        
        void dispatchAfter(double delay, Handler&& handler)
        {
            TimerManager::createTimer(std::forward<Handler>(handler), io_context_, delay);
        }
        
    private:
        asio::io_context& io_context_;
        std::shared_ptr<ExecutorGuard> guard_;
    };
    
    class GlobalQueue
    {
    public:
        
        static GlobalQueue& Instance()
        {
            static GlobalQueue queue;
            return queue;
        }
        
        GlobalQueue()
        : pool_(4)
        {}
        
        void post(Handler&& handler)
        {
            
            asio::post(pool_, std::forward<Handler>(handler));
        }
        
        void dispatch(Handler&& handler)
        {
            asio::dispatch(pool_, std::forward<Handler>(handler));
        }
        
        void strand(Handler&& handler)
        {
            //            asio::post(asio::strand<decltype(pool_.get_executor())>(), handler);
        }
        
        
    private:
        asio::thread_pool pool_;
    };
}

#if 0

void test_asio_queue()
{
    using namespace Queue;
    
    MainQueue::MainQueueInit();
    
    auto& mainQueue = MainQueue::Instance();
    GlobalQueue& globalQueue = GlobalQueue::Instance();
    
    
    auto block1 = [&]{
        auto tid = std::this_thread::get_id();
        std::cout << " this thread: " << tid << std::endl;
        
        mainQueue.dispatchAfter(3, [&]{
            std::cout << " this thread: " << std::this_thread::get_id() << std::endl;
        });
    };
    
    for (int i = 0; i < 10000; ++i) {
        globalQueue.post(block1);
    }
    
    mainQueue.runMainThread();
    
}

void test_timer()
{
    using BasicTimer = asio::basic_waitable_timer<std::chrono::steady_clock>;
    
    asio::io_context io_context;
    auto guard = asio::make_work_guard(io_context);
    
    Queue::TimerManager::TimerPtr timer(new BasicTimer(io_context));
    
    
    
    std::function<void(std::error_code)> handler;
    handler = [&, timer](std::error_code ec)
    {
        std::cout << time(NULL) << std::endl;
        timer->expires_after(std::chrono::milliseconds(100));
        timer->async_wait(handler);
    };
    timer->expires_after(std::chrono::milliseconds(100));
    timer->async_wait(handler);
    
    
    io_context.run();
}


#endif

}
