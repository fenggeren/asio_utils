//
//  NetBase.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/30.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include "FASIOType.h"

namespace fasio
{

class IConnection
{
public:
	virtual bool isConnected() = 0;

	virtual void send(const char* buf, uint32 len) = 0;

	virtual void disconnect() = 0;

	virtual const uint32 getRemoteIP() = 0;

	virtual const char* getRemoteIPStr() = 0;

	virtual uint16 getRemotePort() = 0;

	virtual const uint32 getLocalIP() = 0;

	virtual const char* getLocalIPStr() = 0;

	virtual uint16 getLocalPort() = 0;

	virtual uint32 getSendBufFree() = 0;
};

class IPacketParser
{
public:
	virtual int32 parsePacket(const char* buf, uint32 len) = 0;
};

class ISession
{
public:

	virtual void setConnection(IConnection* connection) = 0;

	virtual void onEstablish() = 0;

	virtual void onTerminate() = 0;

	virtual bool onError(int32 err, int32 sysErr) = 0;

	virtual void onRecv(const char* buf, int len) = 0;

	virtual void release(void) = 0;
};

}



















