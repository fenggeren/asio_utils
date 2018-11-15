//
//  MSKernel.cpp
//  MatchServer
//
//  Created by huanao on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "MSKernel.hpp"
#include "MSSessionManager.hpp"
#include "GateSession.hpp"
#include "M2CSession.hpp"
#include <CPG/CPGHeader.h>
#include "MSMatchManager.hpp"
#include <CPG/CPGToCentral.pb.h>
#include <Net/Util/ParseProto.hpp>
#include <CPG/Util/ConvFunctional.hpp>

using namespace fasio::logging;


void MSKernel::start()
{
    auto factory = std::make_shared<GateSessionFactory>(g_IoContext);
    SessionManager.createListener(7851, false, factory);
    centralSession_ = SessionManager.createConnector(ServerType_CentralServer, g_IoContext, "127.0.0.1", 7802);
    g_IoContext.run();
    
}

void MSKernel::removeConnectService(int uuid)
{
    connectServices_.erase(uuid);
    
    if (centralSession_ && uuid == centralSession_->uuid())
    {
        centralSession_ = nullptr;
    }
}

void MSKernel::transToCS(const void* data, const PacketHeader& header)
{
    if (centralSession_)
    {
        SessionManager.sendMsg(centralSession_, data, header);
    }
    else
    {
        LOG_ERROR << " not connect login server"
        << " msgid: " << header.type;
    }
}

std::shared_ptr<TCPSession>
MSKernel::connectService(unsigned short type,
                         unsigned short port,
                         unsigned short sid,
                         const std::string& ip)
{
    return SessionManager.createConnector(type, g_IoContext,  ip, port);
}


void MSKernel::checkMatchDistributeRS(const void* buffer, const PacketHeader& header)
{
    LOG_MINFO << "";
    CPGToCentral::CheckMatchDistributeRS rs;
    if (parseProtoMsg(buffer, header.size, rs))
    {
        LOG_MINFO << " result: " << rs.result();
        if (rs.result() == 1 ||
            rs.result() == 2)
        {
            std::list<int> mids;
            for(int i = 0; i < rs.mids_size(); i++)
            {
                mids.push_back(rs.mids(i));
            }
            MSMatchManager::instance().updateMatches(mids);
            LOG_MINFO << " valid mids: " << jointContainer(mids);
        }
    }
}

void MSKernel::distibuteMatchesNotify(const void* buffer, const PacketHeader& header)
{
    LOG_MINFO << "";
    CPGToCentral::ServiceMatchDistibuteNotify notify;
    if (parseProtoMsg(buffer, header.size, notify))
    {
        std::list<int> mids;
        for(int i = 0; i < notify.mid_size(); i++)
        {
            int mid = notify.mid(i);
            mids.push_back(mid);
        }
        MSMatchManager::instance().updateMatches(mids);
        
        LOG_MINFO << "distribute matches: " << jointContainer(mids);
    }
}








