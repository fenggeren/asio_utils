//
//  W2CSession.hpp
//  WebServer
//
//  Created by huanao on 2018/11/22.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/Net/CPGToCentralSession.hpp>

using namespace fasio;

// GateServer -> CentralServer
class W2CSession : public CPGToCentralSession
{
public:
    
    
    
private:
    
    virtual void onClose() override;
    virtual void sendInitData() override;
    
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
    virtual ServiceKernel& serviceKernel() override;
     
private:
    
};







