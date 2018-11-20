//
//  RobotManager.cpp
//  ClientRobots
//
//  Created by guanrui fu on 2018/11/5.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "RobotManager.hpp"
#include "RobotSessionManager.hpp"
#include <CPG/CPGCommon.pb.h>
#include <CPG/CPGClient.pb.h>
#include <Net/FASIO.hpp>


using namespace fasio::logging;


RobotManager::RobotManager()
:ip_("127.0.0.1"), 
port_(7841)
{
    //
    TimerManager::createTimer([&]{
        int idx = arc4random_uniform(robotNums_);
        auto robot = robots_[idx];
        joinMatchRQ(robot);
    }, io_context_, 11, 0.001, 10000000);
    
    TimerManager::createTimer([&]{
        int idx = arc4random_uniform(robotNums_);
        auto robot = robots_[idx];
        unjoinMatchRQ(robot);

    }, io_context_, 13, 0.001, 1000000);
    
}

void RobotManager::start()
{
    robots_.resize(robotNums_);
    for(int i = 0; i < robotNums_; ++i)
    {
        auto robot = std::make_shared<Robot>(ip_, port_);
        auto session = SessionManager.createConnector(ServerType_BalanceServer, io_context_, robot->ip(), robot->port());
        session->setLogicID(i);
        robot->setLogicID(session->logicID());
        robot->setUserInfo({i, session});
        robots_[i] = robot;
        if (i % 100 == 0)
        {
            usleep(10000);
        }
    }
    io_context_.run();
}

std::shared_ptr<Robot>
RobotManager::getRobot(const std::shared_ptr<TCPSession>& session)
{
    std::shared_ptr<Robot> robotPtr = nullptr;
    for(auto& robot : robots_)
    {
        if (robot->logicID() == session->logicID())
        {
            robotPtr = robot;
            break;
        }
    }
    return robotPtr;
}

void RobotManager::postConnect(std::shared_ptr<Robot> robot)
{
    auto session = SessionManager.createConnector(ServerType_GateServer, io_context_, robot->ip(), robot->port());
    session->setLogicID(robot->logicID());
    auto uinfo = robot->userInfo();
    robot->setSession(session);
}

void RobotManager::joinMatchRQ(const std::shared_ptr<Robot>& robot)
{
    CPGCommon::JoinMatchRQ rq;
    rq.set_uid(robot->userInfo().uid);
    
    auto match = robot->randMatch();
    rq.set_mid(match.mid);
    rq.set_mtype(match.type);
    
    PacketHeader header{kJoinMatchRQ, rq.ByteSize(), match.mid};
    SessionManager.sendMsg(robot->userInfo().session,
                           rq.SerializeAsString().data(),
                           header);
}
void RobotManager::unjoinMatchRQ(const std::shared_ptr<Robot>& robot)
{
    CPGCommon::UnjoinMatchRQ rq;
    rq.set_uid(robot->userInfo().uid);
    
    auto match = robot->randMatch();
    rq.set_mid(match.mid);
    rq.set_mtype(match.type);
    
    PacketHeader header{kJoinMatchRQ, rq.ByteSize(), match.mid};
    SessionManager.sendMsg(robot->userInfo().session,
                           rq.SerializeAsString().data(),
                           header);
}
void RobotManager::matchListRQ(const std::shared_ptr<Robot>& robot)
{
    CPGCommon::MatchListRQ rq;
    rq.set_uid(robot->userInfo().uid);
    
    PacketHeader header{kMatchListRQ, rq.ByteSize(), robot->userInfo().uid};
    SessionManager.sendMsg(robot->userInfo().session,
                           rq.SerializeAsString().data(),
                           header);
}


void RobotManager::loginRS(const std::shared_ptr<TCPSession>& session,
             const void* data, const PacketHeader& header)
{
    CPGClient::LoginRS rs;
    if (parseProtoMsg(data, header.size, rs))
    {
        LOG_MINFO << " id: " << session->logicID();
        
        matchListRQ(getRobot(session));
    }
    else
    {
        LOG_ERROR << "";
    }
}
void RobotManager::joinMatchRS(const std::shared_ptr<TCPSession>& session,
                 const void* data, const PacketHeader& header)
{
    CPGCommon::JoinMatchRS rs;
    if (parseProtoMsg(data, header.size, rs))
    {
        auto robot = getRobot(session);
        if (!robot)
        {
            LOG_ERROR << " not found robot: " << session->logicID();
        }
        else
        {
            robot->joinedMatch(rs.mid());
            LOG_MINFO << rs.DebugString();
        }
    }
    else
    {
        LOG_ERROR << "";
    }
}
void RobotManager::unjoinMatchRS(const std::shared_ptr<TCPSession>& session,
                   const void* data, const PacketHeader& header)
{
    CPGCommon::UnjoinMatchRS rs;
    if (parseProtoMsg(data, header.size, rs))
    {
        auto robot = getRobot(session);
        if (!robot)
        {
            LOG_ERROR << " not found robot: " << session->logicID();
        }
        else
        {
            robot->unjoinedMatch(rs.mid());
            LOG_MINFO << rs.DebugString();
        }
    }
    else
    {
        LOG_ERROR << "";
    }
}
void RobotManager::matchListRS(const std::shared_ptr<TCPSession>& session,
                 const void* data, const PacketHeader& header)
{
    LOG_MINFO << "";
    CPGCommon::MatchListRS rs;
    if (parseProtoMsg(data, header.size, rs))
    {
        auto robot = getRobot(session);
        if (!robot)
        {
            LOG_ERROR << " not found robot: " << session->logicID();
        }
        else
        {
            std::vector<MatchBriefInfo> infos;
            for(auto& mi : rs.matchlist())
            {
                MatchBriefInfo info;
                info.mid = mi.mid();
                info.type = mi.type();
                info.time = mi.time();
                info.totalPlayer = mi.totalplayer();
                info.remainedPlayer = mi.remainedplayer();
                info.state = mi.state();
                info.curLevel = mi.curlevel();
                info.entryfee = mi.entryfee();
                infos.push_back(std::move(info));
            }
            robot->setMatches(std::move(infos));
        }
    }
    else
    {
        LOG_ERROR << "";
    }
}



