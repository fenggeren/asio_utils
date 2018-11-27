//
//  CSWebServer.hpp
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <CPG/Third/craw/crow_all.h>

 

class CSWebServer
{
public:
    void start();
    
private:
    
    void route();
    
    
private:
    crow::SimpleApp app;
};

