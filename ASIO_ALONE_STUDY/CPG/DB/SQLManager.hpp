//
//  SQLManager.hpp
//  C++Demo
//
//  Created by guanrui fu on 2018/11/13.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include <mysql.h>
#include <string>

class SQLConnector
{
public:
    
    bool connectDB(const std::string& dbname,
                   const std::string& username,
                   const std::string& pwd,
                   const std::string& ip,
                   int port = 3306);
    
    int execQuery(const std::string_view& data);
    
    bool reConnect();
    
    int errorCode() { return mysql_errno(mysql_); }
    std::string errorStr() { return mysql_error(mysql_); }
    
private:
    MYSQL* mysql_{NULL};
    
    std::string dbname_;
    std::string username_;
    std::string password_;
    std::string ip_;
    int port_{3306};
};


class SQLManager
{
public:
    
    SQLManager()
    {
        
    }
    
private:
    
    MYSQL* mysql_;
};




