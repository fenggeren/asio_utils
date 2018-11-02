//
//  ParseProto.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/29.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <memory>

namespace fasio
{
    
char* getStaticBigMemory();
bool checkMemory();
void resetMemory(bool flag);

template <class T>
class ProtoMsgDeleter
{
public:
    void operator()(T* pointer)
    {
        resetMemory(false);
        pointer->~T();
    }
};

template <class T>
std::shared_ptr<T> parseProtoMsg(const void* data, int len)
{
    if (checkMemory())
    {
        std::shared_ptr<T> msg(new T);
        bool res = msg->ParseFromArray(data, len);
        if (!res)
        {
            return nullptr;
        }
        return msg;
    }
    else
    {
        std::shared_ptr<T> msg(new (getStaticBigMemory())T, ProtoMsgDeleter<T>());
        bool res = msg->ParseFromArray(data, len);
        if (!res)
        {
            return nullptr;
        }
        return msg;
    }
}

template <class T>
bool parseProtoMsg(const void* data, int len, T& msg)
{
    return msg.ParseFromArray(data, len);
}





}

