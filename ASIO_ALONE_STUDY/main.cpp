//
//  main.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/6/10.
//  Copyright © 2018年 fenggeren. All rights reserved.
// 

#include <iostream>
#include <chrono> 
#include "Header.h"
#include <chrono>
#include <type_traits>
#include <sys/syscall.h>
#include "Queue.hpp"  

using namespace asio;
using namespace fasio;
/*
void printNow()
{
    std::cout << "====now====: " << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
}

void test_timers()
{
    constexpr int duration = 30;
    
    asio::io_context io;
    auto guard = asio::make_work_guard(io);
    
    asio::basic_waitable_timer<std::chrono::steady_clock> timer(io, std::chrono::milliseconds(1333));
    timer.expires_after(std::chrono::milliseconds(1333));
    
    std::function<void(std::error_code)> do_block =
    [&](std::error_code ec)
    {
        printNow();
    };
    
    asio::basic_waitable_timer<std::chrono::steady_clock> timer1(io, std::chrono::milliseconds(1111));
    timer1.expires_after(std::chrono::milliseconds(1111));
    printNow();
    auto do_block1 = do_block;
    timer.async_wait(do_block);
    timer1.async_wait(do_block);
    
    
    io.run();
}

#include <boost/asio/yield.hpp>

asio::coroutine cor;

int invoke_coroutine()
{
    std::cout << std::this_thread::get_id() << "           ";
    reenter(cor)
    {
        yield return 1;
        yield return 2;
        yield return 3;
        yield return 4;
        yield return 5;
        yield return 6;
        yield return 7;
        yield return 8;
        yield return 9;
        yield return 10;
        yield return 11;
        yield return 12;
        yield return 13;
        yield return 14;
        yield return 15;
    }
    return 0;
}
#include <boost/asio/unyield.hpp>

void test_coroutine()
{
    std::thread t([]{
        for (int i = 0; i < 15; i++)
        {
            sleep(1);
            std::cout << invoke_coroutine() << std::endl;
        }
    });
    
    for (int i = 0; i < 15; i++)
    {
        sleep(1);
        std::cout << invoke_coroutine() << std::endl;
    }
    
    t.join();
}

struct SwapStruct
{
    int version;
    std::string name;
};


#include "TCPConnector.hpp"
#include "TCPServer.hpp"

//asio::io_context g_io_context;
//using Socket = asio::ip::tcp::socket;
//auto guard = asio::make_work_guard(g_io_context);
//
//void test_network()
//{
//    TCPConnector connector(g_io_context, "127.0.0.1", 18081);
//    connector.setNewConnectionCallback([](std::shared_ptr<Socket> newSocket)
//    {
//        std::cout << " ====== " << std::endl;
//    });
//    connector.start();
//    g_io_context.run();
//}
//
//void test_tcp_server()
//{
//    TCPServer server(g_io_context, 8085);
//    server.start();
//    
//    g_io_context.run();
//}

#include "TCPClient.hpp"

void test_TCPClient()
{
    asio::io_context io_context;
    TCPClient client(io_context, "127.0.0.1", 5058);
    client.connect();
    
    io_context.run();
}

#include "Queue.hpp"

void test_main_Queue()
{
    using namespace Queue;
    
    asio::io_context io_context;
    TimerManager manager(io_context);
    
    manager.createRepeatTimer(1, 0.5, 1000,
                              []{
                                  std::cout << " ======== " << std::endl;
                              }, io_context);
    
    io_context.run();
    
    
    auto& main = MainQueue::Instance();
    
    main.runMainThread();
}

void test_block_blocks()
{
    using Handler = std::function<void(const std::string&)>;
    using Error = std::function<void(int)>;
    
    using AsyncHandler = std::function<void(Handler, Error)>;
    
    AsyncHandler async = [](const Handler& handler,
                            const Error& error)
    {
        std::cout << "begin handler" << std::endl;
        handler("处理");
        error(3);
    };
    
    async([](const std::string& msg){
        std::cout << msg << std::endl;
    },
    [](int err){
        std::cout << "Handler error code: " << err << std::endl;
    });
}

#include "Logging.hpp"

logging::AsyncLogging* g_logging = nullptr;

void asyncOutput(const char* msg, size_t len)
{
    g_logging->append(msg, (int)len);
}

off_t kRollSize = 500*1000*1000;

void testLog()
{
    using namespace logging;
    while (true)
    {
        LOG_MINFO << "    ";
    }
}

void testAsyncLogging()
{
    using namespace logging;
    AsyncLogging log("logging", kRollSize);
    log.start();
    g_logging = &log;
    
    Logger::setOutput(asyncOutput);
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([&]{
            testLog();
        }));
    }
    sleep(10);
    
    for (auto& t : threads)
    {
        t.join();
    }
}

void test_timer()
{
    asio::basic_waitable_timer<std::chrono::steady_clock> timer(getIoContext());
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait([](std::error_code ec){
        std::cout << " ==== " << std::endl;
    });
    getIoContext().run_one();
}

std::string abc("abc");
const char* test_string_data()
{
    return abc.data();
}

using namespace fasio::queue;

void test_main_queue()
{
    
    
    MainQueue.dispatch([]{
        std::cout << "haha" << std::endl;
    });
    
    MainQueue.dispatchAfter(2, []{
        std::cout << "delay haha" << std::endl;
    });
    
    MainQueue.runForever();
}

void test_global_queue()
{
    std::thread thread([]{
        Queue queue;
        
        queue.runForever();
    });
    thread.detach();
}

#include "TCPClient.hpp"
#include "DataBuffer.hpp"
#include "Queue.hpp"
#include "TCPSession.hpp"

class Clients
{
    struct client
    {
        asio::io_context io_context_;
        std::shared_ptr<TCPClient> client_;
    };
public:
    
    void start()
    {
        std::list<std::shared_ptr<std::thread>> threads_;
        for(int i = 0; i < 1; i++)
        {
            threads_.push_back(std::make_shared<std::thread>(([&]{
                createClient();
            })));
        }
        
        for (auto& t : threads_)
        {
            t->join();
        }
    }
    
    void createClient()
    {
        client c;
        c.client_ = std::make_shared<TCPClient>(c.io_context_, "127.0.0.1", 7237);
        c.client_->setMessageCallback(std::bind(&Clients::messageCallback, this, std::placeholders::_1, std::placeholders::_2));
        c.client_->connect();
        c.io_context_.run();
    }
    
private:
    
    void messageCallback(const std::shared_ptr<TCPSession>& session, DataBuffer*const buffer)
    {
        std::string cnt(buffer->peek(), buffer->readableBytes());
        buffer->retrieveAll();
        session->send(cnt);
    }
    
};


*/

int main(int argc, const char * argv[])
{
  
    
    return 0;
}








