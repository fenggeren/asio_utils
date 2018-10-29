//
//  DataBuffer.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/11.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <vector>
#include <assert.h>
#include <stdlib.h>
namespace fasio
{


class DataBuffer
{
public:
    
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;
    
    
    DataBuffer(size_t initialSize = kInitialSize)
    : buffer_(kCheapPrepend + initialSize)
    , readerIndex_(kCheapPrepend)
    , writerIndex_(kCheapPrepend)
    {
        
    }
    
    size_t readableBytes() const
    {
        return writerIndex_ - readerIndex_;
    }
    
    size_t writableBytes() const
    {
        return buffer_.size() - writerIndex_;
    }
    
    size_t prependableBytes() const
    {
        return readerIndex_;
    }
    
    const char* peek() const
    {
        return begin()  + readerIndex_;
    }
    
    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        if (len < readableBytes()) {
            readerIndex_ += len;
        }
        else
        {
            retrieveAll();
        }
    }
    void retrieveInt32()
    {
        retrieve(sizeof(int32_t));
    }
    void retrieveInt16()
    {
        retrieve(sizeof(int16_t));
    }
    void retrieveInt8()
    {
        retrieve(sizeof(int8_t));
    }
    
    void retrieveAll()
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }
    
    /////////
    void append(const char* data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }
    
    void append(const void* data, size_t len)
    {
        append(static_cast<const char*>(data), len);
    }
    
    void ensureWritableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }
    
    
    char* beginWrite()
    {
        return begin() + writerIndex_;
    }
    const char* beginWrite() const
    {
        return begin() + writerIndex_;
    }
    
    char* beginRead()
    {
        return begin() + readerIndex_;
    }
    const char* beginRead() const
    {
        return begin() + readerIndex_;
    }
    
    void hasWritten(size_t len)
    {
        assert(len <= writableBytes());
        writerIndex_ += len;
    }
    
    
    void unwrite(size_t len)
    {
        assert(len <= readableBytes());
        writerIndex_ -= len;
    }
    
    void appendInt32(int32_t x)
    {
        int32_t val = htonl(x);
        append(&val, sizeof(val));
    }
    void appendInt16(int16_t x)
    {
        int16_t val = htonl(x);
        append(&val, sizeof(val));
    }
    void appendInt8(int8_t x)
    {
        int8_t val = htonl(x);
        append(&val, sizeof(val));
    }
    
    /////////
    
    int32_t readInt32()
    {
        int32_t val = peekInt32();
        retrieveInt32();
        return val;
    }
    int16_t readInt16()
    {
        int16_t val = peekInt16();
        retrieveInt16();
        return val;
    }
    int8_t readInt8()
    {
        int8_t val = peekInt8();
        retrieveInt8();
        return val;
    }
    
    /////////
    
    int32_t peekInt32() const
    {
        assert(readableBytes() >= sizeof(int32_t));
        int32_t val;
        ::memcpy(&val, peek(), sizeof(int32_t));
        return ntohl(val);
    }
    
    int16_t peekInt16() const
    {
        assert(readableBytes() >= sizeof(int16_t));
        int16_t val;
        ::memcpy(&val, peek(), sizeof(int16_t));
        return ntohl(val);
    }
    
    int8_t peekInt8() const
    {
        assert(readableBytes() >= sizeof(int8_t));
        int8_t val;
        ::memcpy(&val, peek(), sizeof(int8_t));
        return ntohl(val);
    }
    
    /////////
    void prependInt32(int32_t x)
    {
        int32_t val = htonl(x);
        prepend(&val, sizeof(val));
    }
    void prependInt16(int16_t x)
    {
        int16_t val = htonl(x);
        prepend(&val, sizeof(val));
    }
    void prependInt8(int8_t x)
    {
        int8_t val = htonl(x);
        prepend(&val, sizeof(val));
    }
    
    void prepend(const void* data, size_t len)
    {
        assert(len <= prependableBytes());
        readerIndex_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d+len, begin()+readerIndex_);
    }
    
    void shrink(size_t reserve)
    {
        buffer_.reserve(reserve);
    }
    
    void operate() { isOperating_ = true; }
    void unoperate() { isOperating_ = false;}
    bool isOperate() const { return isOperating_; }
private:
    
    char* begin()
    {
        return &*buffer_.begin();
    }
    
    const char* begin() const
    {
        return &*buffer_.begin();
    }
    
    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() - kCheapPrepend < len)
        {
            buffer_.resize(writerIndex_ + len);
        }
        else
        {
            assert(kCheapPrepend < readerIndex_);
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                      begin() + writerIndex_,
                      begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
            assert(readable == readableBytes());
        }
    }
    
private:
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
    
    bool isOperating_{false};
};



}
