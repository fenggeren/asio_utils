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

class Event;

using EventPtr = std::shared_ptr<Event>;

class Event
{
    
};

class Active
{
public:
    
    Active();
    
    
    void start();
    void stop()
    {
        stop_ = true;
        condition_.notify_all();
    }
    
    void send(Event* event);
    Event* getFreeEvent();

private:
    
    void run();
    
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::shared_ptr<std::thread> thread_;
    std::list<Event*> events_;
    std::list<Event*> swapEvents_;
    std::atomic<bool> stop_{false};
    ThreadSafeObjectPool<Event, Event*> eventPool_;
};













