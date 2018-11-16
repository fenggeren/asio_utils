//
//  M2CSession.hpp
//  MatchServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//
#pragma once
#include <CPG/Net/CPGToCentralSession.hpp>

using namespace fasio;


class M2CSession : public CPGToCentralSession
{
public:
    
    
    void checkMatchDistribute();
    
private:
    
    virtual void sendInitData() override;
    virtual void onClose() override;
private:
     
private:
     
    virtual bool handlerMsg(const std::shared_ptr<TCPSession>& session,
                            const void* buffer,
                            const PacketHeader& header) override;
    virtual ServiceKernel& serviceKernel() override;
private:
    bool firstConnect_{true};
};
