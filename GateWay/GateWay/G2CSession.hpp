//
//  G2CSession.hpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//
#pragma once
#include <CPG/Net/CPGNetSession.hpp>

using namespace fasio;

// GateServer -> CentralServer
class G2CSession : public CPGClientSession
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
    
    void serverRegistRS(const void* data, int len);
    void serverLoginRS(const void* data, int len);
    void newServicesNotify(const void* data, int len);
private:
};
