//
//  HTTPClientManager.hpp
//  C++Demo
//
//  Created by huanao on 2018/11/27.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once
#include <deque>
#include "../Third/simple_web_server/client_http.hpp"


class HTTPClientManager
{
public:
    
    using error_code = SimpleWeb::error_code;
    using CaseInsensitiveMultimap = SimpleWeb::CaseInsensitiveMultimap;
    
    using RequestCallback = std::function<void(int, const std::string&,
                                               const std::string&)>;
    
public:
    
    HTTPClientManager(const std::string& baseURL);
    
    void start();
    
    void syncStart();
    
    void stop();
    
    void GET(const std::string &path,
             const std::string& content,
             const CaseInsensitiveMultimap &header,
             RequestCallback&& callback);
    void POST(const std::string &path,
             const std::string& content,
             const CaseInsensitiveMultimap &header,
             RequestCallback&& callback);
    
    
    
    void request(const std::string &method,
                 const std::string &path,
                 const std::string& content,
                 const CaseInsensitiveMultimap &header,
                 RequestCallback&& callback);
    
    
private:
    
    void run();
    
private:
    using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
    struct Request;
    
    std::deque<std::shared_ptr<Request>> requests_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::unique_ptr<std::thread> thread_;
    const std::string baseURL_;
    std::shared_ptr<HttpClient> baseClient_;
    bool stop_{false};
};













