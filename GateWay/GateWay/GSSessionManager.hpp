//
//  GSSessionManager.hpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp> 
#include <CPG/CPGHeader.h>

using namespace fasio;


#define SessionManager GSSessionManager::instance()

class GSSessionManager : public TCPSessionManager
{
public:
    
    static GSSessionManager& instance()
    {
        static GSSessionManager manager;
        return manager;
    }
    

public:
    
    void transToMatchServer(int32 mid, const google::protobuf::Message& msg, int32 msgType);
    
private:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override;
    
    
private:
    std::unordered_map<uint32, std::shared_ptr<TCPSession>> mid2MatchServers_;
};



