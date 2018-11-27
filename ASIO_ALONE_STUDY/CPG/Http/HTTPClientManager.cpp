//
//  HTTPClientManager.cpp
//  C++Demo
//
//  Created by huanao on 2018/11/27.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "HTTPClientManager.hpp"

struct HTTPClientManager::Request
{
    std::string method;
    std::string path;
    std::string content;
    HTTPClientManager::CaseInsensitiveMultimap header;
    HTTPClientManager::RequestCallback callback;
};


HTTPClientManager::HTTPClientManager(const std::string& baseURL)
: baseURL_(baseURL),
baseClient_(std::make_unique<HttpClient>(baseURL_))
{
    
}

void HTTPClientManager::start()
{
    thread_ = std::make_unique<std::thread>(std::bind(&HTTPClientManager::run, this));
}

void HTTPClientManager::syncStart()
{
    run();
}

void HTTPClientManager::stop()
{
    std::unique_lock<std::mutex> lock(mutex_);
    stop_ = true;
    condition_.notify_one();
}

void HTTPClientManager::run()
{
    while (!stop_)
    {
        std::shared_ptr<Request> req = nullptr;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (requests_.size() == 0 && !stop_)
            {
                condition_.wait(lock, [&]{
                    return requests_.size() > 0 || stop_;
                });
            }
            if (requests_.size() > 0)
            {
                req = requests_.front();
                requests_.pop_front();
            }
        }
        
        if (req && !stop_)
        {
            try
            {
                auto res = baseClient_->request(req->method, req->path,
                                                req->content, req->header);
                req->callback(0, "", res->content.string());
            }
            catch (std::exception& ec)
            {
                req->callback(-1, ec.what(), "");
            }
 
        }
    }
}



void HTTPClientManager::GET(const std::string &path,
                             const std::string& content,
                             const CaseInsensitiveMultimap &header,
                             RequestCallback&& callback)
{
    request("GET", path, content, header,
            std::forward<RequestCallback>(callback));
}
void HTTPClientManager::POST(const std::string &path,
                              const std::string& content,
                              const CaseInsensitiveMultimap &header,
                              RequestCallback&& callback)
{
    request("POST", path, content, header,
            std::forward<RequestCallback>(callback));
}


void HTTPClientManager::request(const std::string &method,
                                 const std::string &path,
                                 const std::string& content,
                                 const CaseInsensitiveMultimap &header,
                                 RequestCallback&& callback)
{
    std::shared_ptr<HTTPClientManager::Request> req(new Request);
    req->method = method;
    req->path = path;
    req->content = content;
    req->header = header;
    req->callback = std::move(callback);
    
    {
        std::unique_lock<std::mutex> lock(mutex_);
        requests_.push_back(std::move(req));
        condition_.notify_one();
    }
}





