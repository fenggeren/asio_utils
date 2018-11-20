//
//  MessageTypeDefine.h
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/19.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once


enum ServiceToService
{
    ServiceToService_Begin = 5000,
    
    kServerRegistRQ = 5001,
    kServerRegistRS = 5002,
    kCheckMatchDistributeRQ = 5003,
    kCheckMatchDistributeRS = 5004,
    kServerNewServicesNotify =  5005,
    kServerAllMatchDistributeNotify = 5006,
    kServerMatchDistributeNotify = 5007,
    
    ServiceToService_End = 10000,
};

enum GSTransFromCP
{
    GSTransFromCP_Begin = 10000,
    GSTransFromCP_End = 16000
};

// 客户端=> GS 直转 LS
enum GSTransToLSFromCP
{
    GSTransToLSFromCP_Begin = 10000,
    
    kClientLoginRQ                = 10001,
    kClientLoginRS                = 10002,
    
    GSTransToLSFromCP_End = 10500,
};
// 客户端=> GS 直转 CS
enum GSTransToCSFromCP
{
    GSTransToCSFromCP_Begin = 11000,
    
    kMatchListRQ            = 11001,
    kMatchListRS            = 11001,
    
    GSTransToCSFromCP_End = 12000,
};

// 客户端=> GS 直转 MS
enum GSTransToMSFromCP
{
    GSTransToMSFromCP_Begin = 15000,
    
    kJoinMatchRQ            = 15001,
    kJoinMatchRS            = 15002,
    kUnjoinMatchRQ          = 15003,
    kUnjoinMatchRS          = 15004,
    
    GSTransToMSFromCP_End = 16000,
};

enum BSTransToCSFromCP
{
    BSTransToCSFromCP_Begin = 17000,
    
    kClientConnectRQ              = 17001,
    kClientConnectRS              = 17002,
    
    BSTransToCSFromCP_End = 17500,
};


enum ClientToGateServer
{
    ClientToGateServer_Begin = 20000,
    ClientToGateServer_End = 20100,
};
enum ClientToBalanceServe
{
    ClientToBalanceServe_Begin = 20500,
    ClientToBalanceServe_End = 21000,
};

































