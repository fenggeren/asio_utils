//
//  CPGNetPacket.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/29.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <memory>

// 消息包
class CPGNetPacket
{
public:
    
    std::shared_ptr<CPGNetPacket>
    createPacket(const char* data, int len, int type);
    
private:
    
};









