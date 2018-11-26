//
//  EventActive.hpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/11/19.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <thread>
#include "ObjectPool.hpp"
#include "TimerManager.hpp"
#include "../Conv.hpp"

namespace fasio
{
    
    
    template <typename T, typename Tptr>
    class EventActive
    {
      using ExecutorGuard = asio::executor_work_guard<asio::io_context::executor_type>;
    public:
        
        using ActiveCallback = std::function<void(const Tptr&)>;
        
        EventActive(const ActiveCallback& cb)
        :eventPool_(1024),
        activeID_(++globalActiveID_),
        callback_(cb)
        {
            guard_ = std::make_shared<ExecutorGuard>(asio::make_work_guard(ioc_));
        }
        
        ~EventActive()
        {
            ioc_.stop();
            if (thread_)
            {
                thread_->join();
                thread_ = nullptr;
            }
        }
        
        
        void setCallback(const ActiveCallback &cb)
        {
            callback_ = cb;
        }
        
        void stop()
        {
            guard_->reset();
        }
        
        void start()
        {
            thread_ = std::make_shared<std::thread>(&EventActive::run, this);
        }
        
        void send(Tptr& t)
        {
            ++pendingWorkNum_;
            asio::dispatch(ioc_, [&]
            {
                callback_(t);
                --pendingWorkNum_;
            });
        }
        
        template<typename ...Value>
        Tptr getFreeEvent(Value&& ...values)
        {
            return eventPool_.createObject(std::forward<Value>(values)...);
        }
        
        
        void releaseEvent(Tptr& event)
        {
            eventPool_.releaseObject(event);
        }
        
        int actorID() const { return activeID_;}
        
    private:
        
        void run()
        {
            ioc_.run();
        }
        
    private:
        std::shared_ptr<std::thread> thread_;
        std::atomic<int> pendingWorkNum_{0};
        int activeID_{0};
        static std::atomic<int> globalActiveID_;
        ThreadSafeObjectPool<T, Tptr> eventPool_;
        ActiveCallback callback_;
        
        asio::io_context ioc_;
        std::shared_ptr<ExecutorGuard> guard_;
    };
    template <typename T, typename Tptr>
    std::atomic<int> EventActive<T, Tptr>::globalActiveID_{0};
    
    
}






