//
//  NetSessionManager.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/30.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <memory>

namespace fasio
{

class TCPSession;

class NetSessionFactory
{
	virtual std::shared_ptr<TCPSession> createSession() = 0;
};


class NetSessionManager : public NetSessionFactory
{
public:
	NetSessionManager();
	virtual ~NetSessionManager();

public:
    
};



}



