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
#include "base/FASIOType.h"

namespace fasio
{

#define INVALID_UUID -1
    
using asio::ip::tcp;
class TCPSession;
class TCPConnector;
using TCPSessionPtr = std::shared_ptr<TCPSession>;
using SocketPtr = std::shared_ptr<tcp::socket>;


class TCPSession : public std::enable_shared_from_this<TCPSession> 
{
public:
    
    TCPSession(SocketPtr socket, const std::string& name = "");
    virtual ~TCPSession();
    
    TCPSession();
    
    // 增加更多数据，不进行发送.
    void addMore(const void* message, size_t len);
    // 直接发送二进制数据
    void send(const void* message, size_t len);
    void send(const std::string& message);
    
    // 异步读取，读取完后调用回调 message callback。
    void startAsyncRead();
    
    // 直接关闭链接
    void shutdown();
    void forceClose(); 
    
    void disconnect()
    { forceClose(); }
    
    // 链接建立 & 销毁
    void connectEstablished();
    void connectDestroyed();
    
public:
    
    void setSocket(SocketPtr socket) { socket_ = socket; }
    void setName(const std::string& name) { name_ = name;}
    
    bool connected() const { return state_ == kConnected ||
                                    state_ == kConnecting; }
    bool disconnected() const { return state_ == kDisconnected ||
                                    state_ == kDisconnecting; }
    
    const std::string& name() const { return name_; }
    
    void setUUID(int32 uuid) { uuid_ = uuid; }
    uint32 uuid() const { return uuid_; }
    
    void setType(int type) {type_ = type;}
    uint8 type() const { return type_;}
    
    bool isClient() const { return client_; }
    
    void setLogicID(uint32 logicID) { logicID_ = logicID; }
    uint32 logicID() const { return logicID_; }
    
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

protected:
    
    virtual void onClose() {}
    virtual void sendInitData() {}
private:
    
    void handRead(std::error_code ec, std::size_t bytesRead);
    void handWrite(std::error_code ec, std::size_t bytesRead);
    void handClose();
    void internalSend(); 
private:
    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
    
protected:
    std::function<void(const TCPSessionPtr&)> writeCompleteCallback_;
    std::function<void(const TCPSessionPtr&)> closeCallback_;
    std::function<void(const TCPSessionPtr&)> connectionCallback_;
    std::function<void(const TCPSessionPtr&, DataBuffer*const)> messageCallback_;
    
    SocketPtr socket_;
    
    DataBuffer* inputBuffer_;
    DataBuffer* outputBuffer_;
    
    DataBuffer* sendBuffer_;
    
    std::string name_;
    
    StateE state_;
    
    uint16 type_{0};
    
    int32 uuid_{INVALID_UUID};
    
    uint32 logicID_{0};
    
    bool client_{false};
    bool retry_{true};
    static std::atomic<int> num_;
};


class ClientSession : public TCPSession
{
public:
    
    ClientSession();
    
    void reconnect();
    
    void stop();
    
    void unenableRetry() { retry_ = false; }
    
    bool canRetry() const { return retry_; }
public:
    
    void setConnector(std::shared_ptr<TCPConnector> connector);
    
    const std::shared_ptr<TCPConnector> connector()
    { return connector_; }
    
private:
    
    void connectCallback(SocketPtr sock);
    
private:
    std::shared_ptr<TCPConnector> connector_{nullptr};
    
    bool connect_{false};
};



}
