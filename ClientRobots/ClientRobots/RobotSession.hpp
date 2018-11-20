//
//  RobotSession.hpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <CPG/Net/CPGNetSession.hpp>
#include <Net/TCPSession.hpp>

using namespace fasio;


class C2BSession : public CPGClientSession
{
public:
    
private:
    
    virtual void onClose() override;
    virtual void sendInitData() override;
    
private:
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
private:
    
    void connectRS(const void* buffer, int len);
};


// GateServer -> MatchServer
class RobotSession : public CPGClientSession
{
public:
public:

    virtual void sendInitData() override;

    virtual void onClose() override;
private:
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
    

private: 
};





