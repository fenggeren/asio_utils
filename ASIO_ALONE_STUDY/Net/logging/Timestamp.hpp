//
//  Timestamp.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/24.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <string>
#include <sys/time.h>

namespace fasio
{

    class Timestamp
    {
    public:
        
        Timestamp()
        : microSecondsSinceEpoch_(0)
        {}
        
        explicit Timestamp(time_t microSecondsSinceEpochArg)
        : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
        {}
        
        std::string toString() const;
        std::string toFormattedString(bool showMicroseconds = true) const;
        
        bool valid() const { return microSecondsSinceEpoch_ > 0; }
        
        time_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
        time_t secondsSinceEpoch() const
        { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }
        
        static Timestamp now();
        static Timestamp invalid()
        {
            return Timestamp();
        }
        
        
        static Timestamp fromUnixTime(time_t t)
        {
            return fromUnixTime(t, 0);
        }
        
        static Timestamp fromUnixTime(time_t t, int microseconds)
        {
            return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
        }
        
        
        
        static const int kMicroSecondsPerSecond = 1000 * 1000;
        
    private:
        time_t microSecondsSinceEpoch_;
    };
    
    inline bool operator<(Timestamp lhs, Timestamp rhs)
    {
        return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
    }
    inline bool operator==(Timestamp lhs, Timestamp rhs)
    {
        return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
    }
    inline double timeDifference(Timestamp high, Timestamp low)
    {
        int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
        return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
    }
    
    inline Timestamp addTime(Timestamp timestamp, double seconds)
    {
        int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
        return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
    }
    
    
    
    
    
    

}
