//
//  GSSessionManager.hpp
//  GateWay
//
//  Created by guanrui fu on 2018/10/31.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSessionManager.hpp>
#include <Net/TCPSession.hpp>
#include <Net/DataBuffer.hpp>
#include <CPG/CPGHeader.h>
#include "ClientSession.hpp"
#include "G2CSession.hpp"
#include "G2MSession.hpp"
#include "G2LSession.hpp"

using namespace fasio;
static asio::io_context g_IoContext;

  

class GSSessionManager : public TCPSessionManager
{
public:
    
    static GSSessionManager& instance()
    {
        static GSSessionManager manager;
        return manager;
    }
    
    void start()
    {
        auto factory = std::make_shared<CGSessionFactory>(g_IoContext);
        createListener(7890, false, factory);
        
        createConnector(ServerType_Gate_Central, g_IoContext, "127.0.0.1", 7801);
        createConnector(ServerType_Gate_Match, g_IoContext, "127.0.0.1", 7851);
        createConnector(ServerType_Gate_Login, g_IoContext, "127.0.0.1", 7831);
        g_IoContext.run();
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



