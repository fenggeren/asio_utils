//
//  TCPConnector.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/12.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "TCPConnector.hpp"

TCPConnector::TCPConnector(asio::io_context& ictx,
             const std::string& ip,
             unsigned short port)
: endpoint_(asio::ip::address_v4::from_string(ip), port)
, connect_(false)
, state_(kDisconnected)
, retryDelayMs_(kInitRetryDelayMs)
{
    
}


void TCPConnector::start(const asio::ip::tcp::socket& sockett)
{
    connect_ = true;
    asio::io_context io;
    asio::ip::tcp::socket socket(io);
    
    socket.async_connect(endpoint_,
                        [this](std::error_code ec)
                        {
                            if (!ec)
                            {
                                
                            }
                        });
}















