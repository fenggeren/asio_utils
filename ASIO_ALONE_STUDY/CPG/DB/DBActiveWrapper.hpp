//
//  DBActiveWrapper.hpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include <string>
#include <memory>
#include <google/protobuf/message.h>
#include "../../Net/Util/NetPacket.hpp"
#include "../../Net/base/FASIOType.h"
#include "SQLConnector.hpp"

namespace fasio
{
    class NetPacket;
    template<class T, class TPtr> class Active;
}

struct DBConfig
{
    unsigned short port;
    std::string ip;
    std::string dbname;
    std::string username;
    std::string pwd;
};

 
// 维护一个线程池。 连接db
class DBActiveWrapper
{
public:
    
    DBActiveWrapper(const DBConfig& config,
                    const  std::function<void(fasio::NetPacket*)>& cb);
    
    bool encodeAndSendToDBThread(google::protobuf::Message& msg,
                                 const PacketHeader& header);
    bool sendToDBThread(const void* data,
                        const PacketHeader& header);
    
    void start();
    
//    std::shared_ptr<SQLConnector> SQLConnector()
//    { return connector_; }
    
    int actorID() const;
private:
    std::unique_ptr<fasio::Active<fasio::NetPacket,
    fasio::NetPacket*>> active_;
    
    std::shared_ptr<SQLConnector> connector_;
    
};




