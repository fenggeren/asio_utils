//
//  TCPSession.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/10.
//  Copyright © 2018年 fenggeren. All rights reserved.
//
#pragma once

#include "asio.hpp"
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "DataBuffer.hpp"


using asio::ip::tcp;
class TCPSession;
using TCPSessionPtr = std::shared_ptr<TCPSession>;
using SocketPtr = std::shared_ptr<tcp::socket>;

class TCPSession : public std::enable_shared_from_this<TCPSession> 
{
public:
    
    TCPSession(SocketPtr socket);
    ~TCPSession();
    void send(const void* message, int len);
    void send(const std::string& message);
    void send(std::string&& message);
    
    void startAsyncRead();
    
    void connect()
    {
        
    }
private:
    
    void handRead(asio::error_code ec, std::size_t bytesRead);
    void handWrite(asio::error_code ec, std::size_t bytesRead);
    void handClose();
    void internalSend();
private:
    SocketPtr socket_;

    std::function<void(const TCPSessionPtr&)> writeCompleteCallback_;
    std::function<void(const TCPSessionPtr&)> closeCallback_;
    std::function<void(const TCPSessionPtr&)> connectionCallback_;
    std::function<void(const TCPSessionPtr&, DataBuffer*const)> messageCallback_;
    
    DataBuffer* inputBuffer_;
    DataBuffer* outputBuffer_;
    
    DataBuffer* sendBuffer_;
     
};






