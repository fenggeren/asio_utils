//
//  Active.cpp
//  C++Demo
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "Active.hpp"

Active::Active()
:eventPool_(1024)
{
}

void Active::start()
{
    thread_ = std::make_shared<std::thread>(&Active::run, this);
}

void Active::send(Event* event)
{
    
}
Event* Active::getFreeEvent()
{
    return eventPool_.createObject();
}

void Active::run()
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
            Event* event = swapEvents_.front();
            swapEvents_.pop_front();
            
            // exec event
            if (event)
            {
                eventPool_.releaseObject(event);
            }
        }
    }
}






