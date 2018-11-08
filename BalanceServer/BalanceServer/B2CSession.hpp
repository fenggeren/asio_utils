//
//  B2CSession.hpp
//  BalanceServer
//
//  Created by huanao on 2018/11/6.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/Net/CPGNetSession.hpp>

using namespace fasio;

// GateServer -> CentralServer
class B2CSession : public CPGClientSession
{
public:
    
private:
    
    virtual void onClose() override;
    virtual void sendInitData() override;
    
private:
    
    
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer, const PacketHeader& header) override;
private:
    
    void serverRegistRS(const void* data, int len);
    void connectRS(const void* data, int len);
private:
};


