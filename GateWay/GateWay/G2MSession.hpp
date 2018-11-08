//
//  G2MSession.hpp
//  GateWay
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//
#pragma once
#include <CPG/Net/CPGNetSession.hpp>

using namespace fasio;

// GateServer -> MatchServer
class G2MSession : public CPGClientSession
{
public:
    virtual void onClose() override;
private:
    
    
    
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
    
private:
};


