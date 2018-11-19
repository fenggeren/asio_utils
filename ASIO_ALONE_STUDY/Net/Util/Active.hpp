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
#include "TimerManager.hpp"
#include "../Conv.hpp"

namespace fasio
{
    

template <typename T, typename Tptr>
class Active
{
    
public:
    
    using ActiveCallback = std::function<void(const Tptr&)>;
    
    Active(const ActiveCallback& cb)
    :eventPool_(1024),
    activeID_(++globalActiveID_),
    callback_(cb)
    {
    }
    
    ~Active()
    {
        condition_.notify_one();
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
        stop_ = true;
        condition_.notify_all();
    }
    
    void start()
    {
        thread_ = std::make_shared<std::thread>(&Active::run, this);
    }
    
    void send(Tptr& t)
    {
        bool needSig = false;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            needSig = events_.empty();
            events_.push_back(t);
        }
        ++pendingWorkNum_;
        if (needSig)
        {
            condition_.notify_one();
        }
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
                auto event(std::move(swapEvents_.front()));
                swapEvents_.pop_front();
                --pendingWorkNum_;
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
    std::list<Tptr> events_;
    std::list<Tptr> swapEvents_;
    std::atomic<bool> stop_{false};
    std::atomic<int> pendingWorkNum_{0};
    int activeID_{0};
    static std::atomic<int> globalActiveID_;
    ThreadSafeObjectPool<T, Tptr> eventPool_;
    ActiveCallback callback_;
};
template <typename T, typename Tptr>
std::atomic<int> Active<T, Tptr>::globalActiveID_{0};


}










