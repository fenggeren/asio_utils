//
//  L2CSession.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <CPG/Net/CPGToCentralSession.hpp>

using namespace fasio;


class L2CSession : public CPGToCentralSession
{
public:
    
private:
    virtual void sendInitData() override;
    virtual void onClose() override;
    virtual ServiceKernel& serviceKernel() override;
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override; 
private:
 
};


