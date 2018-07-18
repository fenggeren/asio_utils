//
//  main.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/6/10.
//  Copyright © 2018年 fenggeren. All rights reserved.
// 

#include <iostream>
#include <chrono>
#include "server.hpp"
#include "asio.hpp"
#include <chrono>
#include <type_traits>
#include <sys/syscall.h>

using namespace asio;


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
    
    std::function<void(asio::error_code)> do_block =
    [&](asio::error_code ec)
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

#include "include/asio/yield.hpp"

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
#include "include/asio/unyield.hpp"

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

int main(int argc, const char * argv[]) {
    
    test_block_blocks();
    
//    test_main_Queue();
    
//    test_network();
//    test_tcp_server();
    
//    test_asio_queue();
    
//    test_timers();
//
//
//    std::pair<uint8_t*, uint8_t> range;
//    range = {};

//    test_server();
    
//    SwapStruct ss1;
//    SwapStruct ss2;
//    
//    ss1.version = 1;
//    ss1.name = "1";
//    ss2.version = 2;
//    ss2.name = "2";
//    
//    std::cout << &ss1 << "      " << &ss2 << std::endl;
//    std::swap(ss1, ss2);
//    std::cout << &ss1 << "      " << &ss2 << std::endl;
    
    return 0;
}








