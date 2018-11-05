//
//  NetPacket.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "NetPacket.hpp"
#include "../base/FASIOType.h"
#include "ParseProto.hpp"

namespace fasio
{
    
NetPacket::NetPacket(const void* data, int size)
:buffer_(new char[size]),
size_(size)
{
    memcpy(buffer_, data, size);
}
    
NetPacket::NetPacket(const void* data, int size, int msgid)
:buffer_(new char[size+kPacketHeaderSize]),
size_(size + kPacketHeaderSize)
{
    PacketHeader header{msgid, size};
    memcpy(buffer_, &header, kPacketHeaderSize);
    memcpy(buffer_+kPacketHeaderSize, data, size);
}

NetPacket::~NetPacket()
{
    if (buffer_)
    {
        delete []buffer_;
        buffer_ = NULL;
    }
}

NetPacketPtr NetPacket::getPacket(const void* data,int len)
{
    NetPacketPtr packet = nullptr;
    if (len > kPacketHeaderSize)
    {
        PacketHeader* header = (PacketHeader*)data;
        if (len >= header->size + kPacketHeaderSize)
        {
            packet = std::make_shared<NetPacket>(data ,header->size + kPacketHeaderSize);
        }
    }
    
    return packet;
}
    
NetPacketPtr NetPacket::createPacket(const void* data,int len, int msgid)
{
    return NetPacketPtr(new NetPacket(data, len, msgid));
}
    
NetPacketPtr NetPacket::createPacket(const std::string& str, int msgid)
{
    return NetPacketPtr(new NetPacket(str.data(), str.size(), msgid));
}
    

template <class T>
std::shared_ptr<T> getProtoMsg(const char* data, int len)
{
    if (len > kPacketHeaderSize)
    {
        PacketHeader* header = (PacketHeader*)data;
        if (len >= header->size + kPacketHeaderSize)
        {
            return parseProtoMsg<T>(data, len);
        }
    }
    return nullptr;
}

bool hasPacket(const char* data, int len)
{
    if (len > kPacketHeaderSize)
    {
        PacketHeader* header = (PacketHeader*)data;
        if (len >= header->size + kPacketHeaderSize)
        {
            return true;
        }
    }
    return false;
}
 
}
