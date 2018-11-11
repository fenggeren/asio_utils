//
//  Active.hpp
//  C++Demo
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//


#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <thread>
#include "ObjectPool.hpp"


template <typename T>
class Active
{
    using ActiveCallback = std::function<void(T*)>;
public:
    
    
    Active(const ActiveCallback& cb)
    :eventPool_(1024),
    callback_(cb)
    {
    }
    
    void setCallback(const ActiveCallback &cb)
    {
        callback_ = cb;
    }
    
    void start()
    {
        thread_ = std::make_shared<std::thread>(&Active::run, this);
    }
    
    void send(T* event)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        events_.push_back(event);
        condition_.notify_one();
    }
    
    template<typename ...Value>
    T* getFreeEvent(Value&& ...values)
    {
        return eventPool_.createObject(std::forward<Value>(values)...);
    }
    
private:
    
    void run()
    {
        while (!stop_)
        {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                while (events_.empty())
                {
                    condition_.wait(lock);
                }
                swapEvents_.swap(events_);
            }
            
            while (!swapEvents_.empty())
            {
                T* event = swapEvents_.front();
                swapEvents_.pop_front();
                
                // exec event
                callback_(event);
                if (event)
                {
                    eventPool_.releaseObject(event);
                }
            }
        }
    }
    
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::shared_ptr<std::thread> thread_;
    std::list<T*> events_;
    std::list<T*> swapEvents_;
    std::atomic<bool> stop_{false};
    ThreadSafeObjectPool<T, T*> eventPool_;
    ActiveCallback callback_;
};













