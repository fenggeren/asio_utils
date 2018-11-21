//
//  FASIOType.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/30.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once


typedef unsigned char		 uchar;
typedef signed char 		int8;
typedef unsigned char		 uint8;

typedef signed short 		int16;
typedef unsigned short 		uint16;

typedef signed int 			int32;
typedef unsigned int 		uint32;

typedef signed long long 	int64;
typedef unsigned long long 	uint64;



#define kClientHeartBeatDuration 9
#define kClientHeartBeatOvertime 20
#define kServerHeartBeatDuration 5
#define kServerHeartBeatOvertime 11

struct PacketHeader
{
    int32 type{0};
    int32 size{0};
    int32 extraID{0};
};


#define kPacketHeaderSize sizeof(PacketHeader)


