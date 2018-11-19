//
//  DBActiveWrapper.cpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "DBActiveWrapper.hpp"
#include "../../Net/FASIO.hpp"

using namespace fasio;
using namespace logging;

DBActiveWrapper::DBActiveWrapper(const DBConfig& config,
                    const std::function<void(NetPacket*)>& cb)
:active_(std::make_unique<Active<NetPacket, NetPacket*>>(cb))
, connector_(new SQLConnector())
{
    int ret = connector_->connectDB(config.dbname,
                                    config.username,
                                    config.pwd,
                                    config.ip, config.port);
    if (ret != 0)
    {
        LOG_ERROR << " cant connect db, ret: " << ret
        << "info: " << config.ip << ":" << config.port
        << " uname: " << config.username
        << " pwd: " << config.pwd;
    }
    else
    {
        LOG_MINFO << "connect sql db  ok";
    }
}

bool DBActiveWrapper::encodeAndSendToDBThread(
         google::protobuf::Message& msg,
        const PacketHeader& header)
{
    return sendToDBThread(msg.SerializeAsString().data(), header);
}

bool DBActiveWrapper::sendToDBThread(const void* data,
                            const PacketHeader& header)
{
    auto buffer = active_->getFreeEvent(data, header);
    buffer->setLogicID(active_->actorID());
    active_->send(buffer);
    return true;
}

int DBActiveWrapper::actorID() const
{
    return active_->actorID();
}

void DBActiveWrapper::start()
{
    active_->start();
}
