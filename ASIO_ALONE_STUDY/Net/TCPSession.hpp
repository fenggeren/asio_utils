//
//  TCPSession.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/10.
//  Copyright © 2018年 fenggeren. All rights reserved.
//
#pragma once

#include "Header.h"
//#include "asio.hpp"
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "DataBuffer.hpp"

namespace fasio
{

    
using asio::ip::tcp;
class TCPSession;
class TCPConnector;
using TCPSessionPtr = std::shared_ptr<TCPSession>;
using SocketPtr = std::shared_ptr<tcp::socket>;


class TCPSession : public std::enable_shared_from_this<TCPSession> 
{
public:
    
    TCPSession(SocketPtr socket, const std::string& name);
    virtual ~TCPSession();
    
    // 直接发送二进制数据
    void send(const void* message, size_t len);
    void send(const std::string& message); 
    
    // 异步读取，读取完后调用回调 message callback。
    void startAsyncRead();
    
    
    // 直接关闭链接
    void shutdown();
    void forceClose(); 
    
    // 链接建立 & 销毁
    void connectEstablished();
    void connectDestroyed();
    
    bool connected() const { return socket_ != nullptr; }
    bool disconnected() const { return socket_ == nullptr; }
    
    const std::string& name() const { return name_; }
public:
    ///////  设置回调   /////////
    void setConnectionCallback(const std::function<void(const TCPSessionPtr&)>& cb)
    {
        connectionCallback_ = cb;
    }
    void setCloseCallback(const std::function<void(const TCPSessionPtr&)>& cb)
    {
        closeCallback_ = cb;
    }
    void setMessageCallback(const std::function<void(const TCPSessionPtr&, DataBuffer*const)>& cb)
    {
        messageCallback_ = cb;
    }
    void setWriteCompleteCallback(const std::function<void(const TCPSessionPtr&)>& cb)
    {
        writeCompleteCallback_ = cb;
    }

private:
    
    void handRead(std::error_code ec, std::size_t bytesRead);
    void handWrite(std::error_code ec, std::size_t bytesRead);
    void handClose();
    void internalSend(); 
private:
    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
    
    
    
protected:
    
    SocketPtr socket_;
    
    std::function<void(const TCPSessionPtr&)> writeCompleteCallback_;
    std::function<void(const TCPSessionPtr&)> closeCallback_;
    std::function<void(const TCPSessionPtr&)> connectionCallback_;
    std::function<void(const TCPSessionPtr&, DataBuffer*const)> messageCallback_;
    
    DataBuffer* inputBuffer_;
    DataBuffer* outputBuffer_;
    
    DataBuffer* sendBuffer_;
    
    const std::string name_;
    
    StateE state_;

};


class ClientSession : public TCPSession
{
public:
    
    
    void setConnector(std::shared_ptr<TCPConnector> connector)
    { connector_ = connector; }
    
    const std::shared_ptr<TCPConnector> connector()
    { return connector_; }
private:
    std::shared_ptr<TCPConnector> connector_{nullptr};
};



}
