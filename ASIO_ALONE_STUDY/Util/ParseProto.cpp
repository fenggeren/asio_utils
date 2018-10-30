//
//  ParseProto.cpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/29.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "ParseProto.hpp"

char* bigMemory = new char[1024*1024];
char* getStaticBigMemory()
{
    return bigMemory;
}
volatile bool bigMemoryIsUse = false;
bool checkMemory()
{
    return bigMemoryIsUse;
}
void resetMemory(bool flag)
{
    bigMemoryIsUse = flag;
}
