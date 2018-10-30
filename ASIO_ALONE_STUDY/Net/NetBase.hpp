//
//  NetBase.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/30.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

namespace fasio
{

class IConnection
{
public:
	virtual bool isConnected() = 0;
	virtual void send(const char* buf, )
};

}

