//
//  SQLManager.cpp
//  C++Demo
//
//  Created by guanrui fu on 2018/11/13.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include "SQLManager.hpp"


bool SQLConnector::connectDB(const std::string& dbname,
                           const std::string& username,
                           const std::string& pwd,
                           const std::string& ip,
                           int port)
{
    mysql_ = mysql_init(NULL);
    if (mysql_ == NULL)
    {
        return false;
    }
    char value = 1;
    mysql_options(mysql_, MYSQL_OPT_RECONNECT, &value);
    mysql_options(mysql_, MYSQL_SET_CHARSET_NAME, "utf8");
    
    bool flag = mysql_real_connect(mysql_, ip.data(), username.data(),
                                   pwd.data(), dbname.data(),
                                   port,
                                   NULL, CLIENT_MULTI_STATEMENTS);
    if (!flag)
    {
        printf("connect error: %s", errorStr().data());
        return false;
    }
    
    ip_ = ip;
    username_ = username;
    password_ = pwd;
    dbname_ = dbname_;
    port_ = port;
    
    return true;
}

bool SQLConnector::reConnect()
{
    mysql_ = mysql_init(NULL);
    if (mysql_ == NULL)
    {
        return false;
    }
    char value = 1;
    mysql_options(mysql_, MYSQL_OPT_RECONNECT, &value);
    mysql_options(mysql_, MYSQL_SET_CHARSET_NAME, "utf8");
    
    bool flag = mysql_real_connect(mysql_, ip_.data(),
                                   username_.data(),
                                   password_.data(),
                                   dbname_.data(),
                                   port_,
                                   NULL, CLIENT_MULTI_STATEMENTS);
    
    return flag;
}
