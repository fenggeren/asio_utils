//
//  TimerManager.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//


#pragma once
#include "Header.h"

namespace fasio
{
    
#define S2M(S) (int)(S*1000)
    
    class TimerManager
    {
        using TimerHandler = std::function<void()>;
        using BasicTimer = asio::basic_waitable_timer<std::chrono::steady_clock>;
    public:
        using TimerPtr = std::shared_ptr<BasicTimer>;
        
        TimerManager(asio::io_context& ioc)
        :io_context_(ioc)
        {}
        
        
        
        static TimerPtr createTimer(TimerHandler&& handler,
                                    asio::io_context& io,
                                    double delay,
                                    double interval = 2,
                                    int count = 1);
        
        TimerPtr createTimer(TimerHandler&& handler,
                             double delay,
                             double interval = 2,
                             int count = 1);
        
        
        
    private:
        
        struct RepeatHandler
        {
            RepeatHandler(TimerPtr timer, double interval, int count,
                          TimerHandler&& handler)
            : timer_(timer)
            , interval_(interval)
            , count_(count)
            , handler_(handler)
            {
            }
            
            ~RepeatHandler()
            {
                printf("%ld\n",  timer_.use_count());
            }
            
            void operator()(std::error_code ec)
            {
                handler_();
                if (--count_ > 0)
                {
                    timer_->expires_after(std::chrono::milliseconds(S2M(interval_)));
                    timer_->async_wait(std::forward<RepeatHandler>(*this));
                }
            }
            
            TimerPtr timer_;
            double interval_;
            int count_;
            TimerHandler handler_;
        };
        
        asio::io_context& io_context_;
    };

}






