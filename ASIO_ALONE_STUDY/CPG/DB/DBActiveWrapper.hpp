//
//  DBActiveWrapper.hpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "../../Net/Util/Active.hpp"
#include <string>

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
    
    DBActiveWrapper(const DBConfig& config);
    
private:
    
    
};




