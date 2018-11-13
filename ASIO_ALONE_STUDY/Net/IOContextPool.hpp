//
//  IOContextPool.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/13.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <vector>
#include <thread>
#include "Header.h"

class IOContextPool
{
    using ExecutorGuard = asio::executor_work_guard<asio::io_context::executor_type>;
public:
    
    IOContextPool(int num)
    :ioContexts_(num)
    {
        for(auto& ctx : ioContexts_)
        {
            guards_.push_back(asio::make_work_guard(ctx));
        }
    }
    
    
    void start()
    {
        for(auto& ctx : ioContexts_)
        {
            threads_.push_back(std::thread([&]{
                ctx.run();
            }));
        }
        for(auto& thread : threads_)
        {
            thread.join();
        }
    }
    
    void stop()
    {
        for(auto& guard : guards_)
        {
            guard.reset();
        }
        for(auto& ctx : ioContexts_)
        {
            ctx.stop();
        }
    }
    
    asio::io_context& getNextIOContext()
    {
        int index = curIndex_++;
        curIndex_ %= ioContexts_.size();
        return ioContexts_[index];
    }
    asio::io_context& getIOContext(int index)
    {
        assert(index>=0);
        return ioContexts_[index % ioContexts_.size()];
    }
    
private:
    std::vector<asio::io_context> ioContexts_;
    std::vector<ExecutorGuard> guards_;
    std::vector<std::thread> threads_;
    int curIndex_{0};
};




