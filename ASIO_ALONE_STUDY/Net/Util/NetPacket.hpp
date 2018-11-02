//
//  NetPacket.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <memory>
#include <string>

namespace fasio
{
class NetPacket;

using NetPacketPtr = std::shared_ptr<NetPacket>;

class NetPacket
{
public:
    
    static NetPacketPtr getPacket(const void* data,int len);
    static NetPacketPtr createPacket(const void* data,int len, int msgid);
    static NetPacketPtr createPacket(const std::string& data, int msgid);
    
    NetPacket(const void* data, int size);
    NetPacket(const void* data, int size, int msgid);
    ~NetPacket();
    
    const void* buffer() const { return buffer_;}
    size_t size() const { return size_;}

private:
    char* buffer_;
    size_t size_;
};


// len是数据总大小，不是proto的大小，
// 如果len过小，这里返回nullptr;
template <class T>
std::shared_ptr<T> getProtoMsg(const char* data, int len);












}
