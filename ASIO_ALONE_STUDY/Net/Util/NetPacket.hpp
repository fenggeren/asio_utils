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
#include <boost/any.hpp>
#include "../base/FASIOType.h"
namespace fasio
{
class NetPacket;

using NetPacketPtr = std::shared_ptr<NetPacket>;

class NetPacket
{
public:
    
    static NetPacketPtr getPacket(const void* data,int len);
    static NetPacketPtr createPacket(const void* data,int len, int msgid, int extraid = 0);
    static NetPacketPtr createPacket(const std::string& data, int msgid, int extraid = 0);
    
    
    NetPacket(const void* data, int size);
    NetPacket(const void* data, const PacketHeader& header);
    NetPacket(const void* data, int size, int msgid, int extraid = 0);
    ~NetPacket();
    
    const void* buffer() const { return buffer_;}
    size_t size() const { return size_;}
    
    void setLogicID(int logicID) { logicID_ = logicID;}
    int logicID() const { return logicID_; }
    
//    void setExtra(const boost::any& any) { extra_ = any;}
//    boost::any& extra() { return extra_;}
private:
    int logicID_;
    char* buffer_;
    size_t size_;
//    boost::any extra_;
};


// len是数据总大小，不是proto的大小，
// 如果len过小，这里返回nullptr;
template <class T>
std::shared_ptr<T> getProtoMsg(const char* data, int len);

bool hasPacket(const char* data, int len);


 
}
