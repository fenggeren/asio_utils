//
//  PromiseFutureQueue.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <future>
#include <map>
#include <boost/any.hpp>
#include <mutex>

struct PromiseProfile
{
    int promiseID;
    std::promise<int> promise;
    boost::any data;
};

class PromiseFutureQueue
{
public:
    
    static PromiseFutureQueue& instance()
    {
        static PromiseFutureQueue queue;
        return queue;
    }
    
    PromiseProfile&
    nextPromise();
    
    boost::any& futureData(int promiseID);
    
    bool setData(const boost::any& data, int value, int promiseID);
    void removePromise(int promiseID);
private:
    std::map<int, PromiseProfile> promiseMap_;
    std::mutex mutex_;
    
    static int promiseID_;
};

