//
//  ActiveBuffer.hpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/11/19.
//  Copyright © 2018年 fenggeren. All rights reserved.
//
#pragma once

// Header + Body + ActiveID
class ActiveBuffer
{
public:
    
    ActiveBuffer(const void* data, int size)
    {
        
    }
    ActiveBuffer(const void* data, int size, int msgid, int extraid = 0)
    {
        
    }
    ~ActiveBuffer()
    {
        if (buffer_)
        {
            delete []buffer_;
            buffer_ = nullptr;
        }
    }
    
    const void* buffer() const { return buffer_;}
    int size() const { return size_;}
    
private:
    int activeID_;
    char* buffer_;
    int size_;
};



