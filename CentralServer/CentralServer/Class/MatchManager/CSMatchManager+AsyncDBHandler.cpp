//
//  CSMatchManager+AsyncDBHandler.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSMatchManager.hpp"
#include <Net/FASIO.hpp>
#include <CPG/CPGServer.pb.h>
#include <CPG/GameDefine.h>
#include <CPG/ErrorCodeDefine.h>
#include <sstream>
#include <CPG/MatchHelper/CPGMatchCreateFactory.hpp>
#include <CPG/MatchHelper/CPGMatchStructHelper.hpp>
#include "PromiseFutureQueue.hpp"

using namespace fasio;
using namespace queue;

void CSMatchManager::dbAsyncHandler(fasio::NetPacket* packet)
{
    PacketHeader* header = (PacketHeader*)packet->buffer();
    const void* data = (const char*)packet->buffer() + kPacketHeaderSize;
    
    switch (header->type)
    {
        case kCancelMatchRQ:
        {
            cancelMatch(data, *header);
            break;
        }
        case kUpdateMatchRQ:
        {
            updateMatch(data, *header);
            break;
        }
        case kCreateMatchRQ:
        {
            createMatch(data, *header);
            break;
        }
        default:
            break;
    }
}


void CSMatchManager::cancelMatch(const void* data,
                                 const PacketHeader& header)
{
    CPGServer::CancelMatchRQ rq;
    if (parseProtoMsg(data, header.size, rq))
    {
        std::stringstream ss;
        ss << "UPDATE match_infos SET ";
        ss << " end_time=" << time(NULL) << " , "
        << "match_state" << "=" << ENUM_STATE_CANCEL
        << " WHERE mid=" << rq.mid();
        
        auto connector = dbwrapper_->connector();
        connector->execQuery(ss.str());
    }

}
void CSMatchManager::updateMatch(const void* data,
                                 const PacketHeader& header)
{
    CPGServer::UpdateMatchRQ rq;
    if (parseProtoMsg(data, header.size, rq))
    {
        std::stringstream ss;
        ss << "UPDATE match_infos SET ";
        
        bool first = true;
        for(auto& pro : rq.properties())
        {
            if(!first) ss << ", "; first = false;
            ss << pro.first << "=" << "'" << pro.second << "'";
        }
        ss << " WHERE mid=" << rq.mid();
        
        auto connector = dbwrapper_->connector();
        connector->execQuery(ss.str());
    }
}
void CSMatchManager::createMatch(const void* data,
                                 const PacketHeader& header)
{
    CPGServer::CreateMatchRQ rq;
    if (parseProtoMsg(data, header.size, rq))
    {
        std::map<std::string, std::string> properties;
        for(auto& pro : rq.properties())
        {
            properties[pro.first] = pro.second;
        }
        CPGMatchProfile profile;
        mapConvertStruct(properties, profile);
        
        auto connector = dbwrapper_->connector();
        int mid = CPGMatchCreateFactory::createAndStoreMatch(profile, *connector.get());
        profile.mid = mid;
        
        auto newproperties = structConvertMap(profile);
        auto newmatch = std::make_shared<CPGMatchProfile>(profile);
        
        MainQueue.dispatch([&]{
            this->createdMatches({newmatch});
        });
        
        PromiseFutureQueue::instance().setData(profile, kNoneError,
                                               header.extraID);
    }
}




