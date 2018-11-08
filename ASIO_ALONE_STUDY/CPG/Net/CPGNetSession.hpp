//
//  CPGNetSession.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/8.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include "../../Net/TCPSession.hpp"
using namespace fasio;
struct PacketHeader;

class CPGServerSession : public TCPSession
{
public:
    CPGServerSession(const SocketPtr& sock, const std::string& name = "");
    
private:
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data);
protected:
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header) = 0;
};


class CPGClientSession : public ClientSession
{
public:
    CPGClientSession();
    
private:
    void defaultMessageCallback(const std::shared_ptr<TCPSession>& session,
                                DataBuffer*const data);
protected:
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header) = 0;
};


