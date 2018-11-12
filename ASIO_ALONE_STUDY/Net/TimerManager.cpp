//
//  TimerManager.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TimerManager.hpp"

namespace fasio
{
    
TimerManager::TimerPtr TimerManager::createTimer(TimerHandler&& handler,
                                                 asio::io_context& io,
                                                 double delay,
                                                 double interval,
                                                 int count)
{
    TimerPtr timer(new BasicTimer(io));
    RepeatHandler timerConfig(timer, interval, count, std::forward<TimerHandler>(handler));
    
    timer->expires_after(std::chrono::milliseconds(S2M(delay)));
    timer->async_wait(std::move(timerConfig));
    
    return timer;
}

TimerManager::TimerPtr
TimerManager::createTimer(TimerHandler&& handler,
                          double delay,
                          double interval,
                          int count)
{
    return TimerManager::createTimer(std::forward<TimerHandler>(handler),
                                     io_context_,delay, interval, count);
}

}
