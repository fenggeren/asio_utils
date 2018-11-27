//
//  PromiseFutureQueue.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "PromiseFutureQueue.hpp"

int PromiseFutureQueue::promiseID_ = 100;


PromiseProfile&
PromiseFutureQueue::nextPromise()
{
    std::lock_guard<std::mutex> lock(mutex_);
    int pid = promiseID_++;
    promiseMap_[pid].promiseID = pid;
    return promiseMap_[pid];
}
 

boost::any&
PromiseFutureQueue::futureData(int promiseID)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return promiseMap_[promiseID].data;
}
bool PromiseFutureQueue::setData(const boost::any& data,
                                 int value, int promiseID)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = promiseMap_.find(promiseID);
    if (iter == promiseMap_.end())
    {
        return false;
    }
    iter->second.data = data;
    iter->second.promise.set_value(value);
    return true;
}

void PromiseFutureQueue::removePromise(int promiseID)
{
    std::lock_guard<std::mutex> lock(mutex_);
    promiseMap_.erase(promiseID);
}









