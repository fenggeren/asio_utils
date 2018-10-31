//
//  Robot.hpp
//  ClientRobots
//
//  Created by huanao on 2018/10/31.
//  Copyright © 2018年 fgr. All rights reserved.
//


#pragma once

#include <Net/TCPSessionFactory.h>
#include <Net/TCPSession.hpp>
#include <Net/TCPConnector.hpp>
#include <Net/TCPClient.hpp>
#include <list>
#include <Net/Queue.hpp>

using namespace fasio;
using namespace queue;
class Robots
{
    using ClientPtr = std::shared_ptr<TCPClient>;
public:
    
    Robots()
    {
        createClients(7890, 10);
        createClients(7891, 10);
        createClients(7892, 10);
    }
    
    void start()
    {
        for (auto& client : clients_)
        {
            client->connect();
        }
        
        auto guard = asio::make_work_guard(io_context_);
        
        TimerManager::createRepeatTimer(5, 5, 10000000, [&]{
            printf("%llu\n", count_);
        }, io_context_);
        
        io_context_.run();
    }
    
private:
    
    void createClients(uint16 port, int num)
    {
        for(int i = 0; i < num; i++)
        {
            ClientPtr client(new TCPClient(io_context_, "127.0.0.1", port));
            client->setMessageCallback(std::bind(&Robots::messageIn, this, std::placeholders::_1, std::placeholders::_2));
            client->setConnectionCallback(std::bind(&Robots::connectCallback, this, std::placeholders::_1));
            clients_.push_back(client);
        }
    }
    
    void messageIn(const std::shared_ptr<TCPSession>& session,
                   DataBuffer*const data)
    {
        count_++;
        std::string content(data->peek(), data->readableBytes());
        data->retrieveAll();
        session->send(content);
    }
    
    void connectCallback(const std::shared_ptr<TCPSession>& session)
    {
        session->send("hello world");
    }
    
private:
    asio::io_context io_context_;
    std::list<ClientPtr> clients_;
    
    unsigned long long count_;
};



