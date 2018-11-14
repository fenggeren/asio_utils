//
//  SQLManager.hpp
//  C++Demo
//
//  Created by guanrui fu on 2018/11/13.
//  Copyright © 2018年 guanrui fu. All rights reserved.
//

#include <mysql.h>
#include <string>
#include <vector>
#include <map>

class SQLConnector
{
public:
    
    int execQuery(const std::string& data);
    
    void fieldData(const std::string& name, int32_t& value);
    void fieldData(const std::string& name, uint32_t& value);
    void fieldData(const std::string& name, int64_t& value);
    void fieldData(const std::string& name, uint64_t& value);
    void fieldData(const std::string& name, std::string& value);
    
    void queryFields();
    void queryNext();
    void closeQuery();
    
    
public:
    
    bool connectDB(const std::string& dbname,
                   const std::string& username,
                   const std::string& pwd,
                   const std::string& ip,
                   int port = 3306);
    
    
    bool reConnect();
    
    void clear();
    void clearResult();
    
    int errorCode() { return mysql_errno(mysql_); }
    std::string errorStr() { return mysql_error(mysql_); }
    
    int queryRowNum() const { return totalRows_; }
    int queryFieldNum() const { return totalFields_; }
    
    MYSQL* mysql() const { return mysql_; }
    MYSQL_RES* myresult() const { return myResult_; }
private:
    MYSQL* mysql_{NULL};
    
    MYSQL_RES* myResult_;
    int totalRows_;
    int totalFields_; 
    std::vector<std::string> fieldVec_;
    std::map<std::string, std::string> fieldsValue_;
private:
    
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




