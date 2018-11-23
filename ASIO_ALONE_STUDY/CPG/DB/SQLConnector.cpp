//
//  SQLConnector.cpp
//  ASIO_ALONE_STUDY
//
//  Created by guanrui fu on 2018/11/19.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "SQLConnector.hpp"


SQLConnector::~SQLConnector()
{
    clear();
}

void SQLConnector::clear()
{
    if (mysql_)
    {
        mysql_close(mysql_);
        mysql_ = NULL;
    }
    if (myResult_)
    {
        myResult_ = NULL;
    }
}
void SQLConnector::clearResult()
{
    bool needQueryNext = (myResult_ != NULL);
    if (myResult_)
    {
        mysql_free_result(myResult_);
    }
    
    if (needQueryNext)
    {
        while (mysql_next_result(mysql_) == 0)
        {
            myResult_ = mysql_store_result(mysql_);
            if (myResult_)
            {
                mysql_free_result(myResult_);
            }
        }
    }
    else
    {
        do
        {
            myResult_ = mysql_store_result(mysql_);
            if (myResult_)
            {
                mysql_free_result(myResult_);
            }
        } while (mysql_next_result(mysql_) == 0);
    }
    myResult_ = NULL;
}

int SQLConnector::execQuery(const std::string& data)
{
    if (data.size() < 1)
    {
        return -1;
    }
    if (mysql_ == NULL)
    {
        if (!reConnect())
        {
            return -1;
        }
    }
    clearResult();
    
    int ret = mysql_real_query(mysql_, data.data(), data.size());
    if (ret != 0)
    {
        return -1;
    }
    
    if (data.find("SELECT") != -1 || data.find("select") != -1)
    {
        myResult_ = mysql_store_result(mysql_);
        totalFields_ = mysql_num_fields(myResult_);
        totalRows_ = static_cast<int>(mysql_affected_rows(mysql_));
        queryFields();
    }
    return ret;
}

 
void SQLConnector::fieldData(const std::string& name, int32_t& value)
{
    auto iter = fieldsValue_.find(name);
    if (iter != fieldsValue_.end())
    {
        value = atoi(iter->second.data());
    }
}
void SQLConnector::fieldData(const std::string& name, uint32_t& value)
{
    auto iter = fieldsValue_.find(name);
    if (iter != fieldsValue_.end())
    {
        value = atoi(iter->second.data());
    }
}
void SQLConnector::fieldData(const std::string& name, int64_t& value)
{
    auto iter = fieldsValue_.find(name);
    if (iter != fieldsValue_.end())
    {
        value = atoll(iter->second.data());
    }
}
void SQLConnector::fieldData(const std::string& name, uint64_t& value)
{
    auto iter = fieldsValue_.find(name);
    if (iter != fieldsValue_.end())
    {
        value = atoll(iter->second.data());
    }
}
void SQLConnector::fieldData(const std::string& name, std::string& value)
{
    auto iter = fieldsValue_.find(name);
    if (iter != fieldsValue_.end())
    {
        value = iter->second;
    }
}

void SQLConnector::queryFields()
{
    fieldVec_.clear();
    fieldsValue_.clear();
    
    MYSQL_FIELD* field = NULL;
    MYSQL_ROW curRow = mysql_fetch_row(myResult_);
    if (!curRow) {
        return;
    }
    int i = 0;
    while ((field = mysql_fetch_field(myResult_)))
    {
        auto tempRes = curRow[i];
        if (tempRes)
        {
            std::string fieldStr(field->name, field->name_length);
            fieldsValue_[fieldStr] = tempRes;
            fieldVec_.push_back(std::move(fieldStr));
        }
        i++;
    }
}
void SQLConnector::queryNext()
{
    fieldsValue_.clear();
    MYSQL_ROW curRow = mysql_fetch_row(myResult_);
    if (!curRow) {
        return;
    }
    auto size = fieldVec_.size();
    for(int i = 0; i < size; ++i)
    {
        auto tempRes = curRow[i];
        if (tempRes)
        {
            fieldsValue_[fieldVec_[i]] = tempRes;
        }
    }
}
void SQLConnector::closeQuery()
{
    fieldsValue_.clear();
}


bool SQLConnector::connectDB(const std::string& dbname,
                             const std::string& username,
                             const std::string& pwd,
                             const std::string& ip,
                             int port)
{
    clear();
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
