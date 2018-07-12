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
, io_context_(ictx)
, connect_(false)
, state_(kDisconnected)
, retryDelayMs_(kInitRetryDelayMs)
{
    
}


void TCPConnector::start()
{
    connect_ = true;
    
    std::shared_ptr<asio::ip::tcp::socket>
    socketPtr(new asio::ip::tcp::socket(io_context_));
    
    socketPtr->async_connect(endpoint_,
                             [=]
                             (std::error_code ec)
                        {
                            if (!ec)
                            {
                                newConnectionCallback_(socketPtr);
                            }
                        });
}















