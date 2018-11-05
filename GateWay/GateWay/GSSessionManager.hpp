//
//  GSSessionManager.hpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp>
#include <Net/DataBuffer.hpp>
#include <CPG/CPGHeader.h>
#include "ClientSession.hpp"
#include "G2CSession.hpp"
#include "G2MSession.hpp"
#include "G2LSession.hpp"


  

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
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type) override
    {
        if (type == ServerType_Gate_Central)
        {
            return std::make_shared<G2CSession>();
        }
        else if (type == ServerType_Gate_Match)
        {
            return std::make_shared<G2MSession>();
        }
        else if (type == ServerType_Gate_Login)
        {
            return std::make_shared<G2LSession>();
        }
        else
        {
            return TCPSessionManager::createConnectorSession(type);
        }
    }
    
    
private:
    std::unordered_map<uint32, std::shared_ptr<TCPSession>> mid2MatchServers_;
};



