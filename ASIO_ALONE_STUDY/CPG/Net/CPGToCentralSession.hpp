//
//  CPGToCentralSession.hpp
//  LoginServer
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include "CPGNetSession.hpp"

namespace fasio
{
    class TCPSessionManager;
}

class CPGToCentralSession : public CPGClientSession
{
public:
    
    virtual ~CPGToCentralSession();
protected:
    
    void sendRegisterData(TCPSessionManager& sessionManager);
    
    void sendHeartBeat(TCPSessionManager& sessionManager); 
private:
    void serverRegistRS(const void* data, int len);
    void newServicesNotify(const void* data, int len);
    
    virtual void defaultMessageCallback(const std::shared_ptr<TCPSession>&      session, DataBuffer*const data);
     virtual ServiceKernel& serviceKernel() = 0;
    
private:
    std::shared_ptr<asio::basic_waitable_timer<
                    std::chrono::steady_clock>> heartTimer_;
    int testID_{0};
};







