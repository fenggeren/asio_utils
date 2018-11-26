//
//  cpg_web_server.hpp
//  reids_demo
//
//  Created by HuanaoGroup on 2018/3/28.
//  Copyright © 2018年 HuanaoGroup. All rights reserved.
//

#pragma once

#include <CPG/Third/craw/crow_all.h>
#include <stdio.h>
#include <vector>


class cpg_web_server
{
public:
    void start();
    
private:
    
    void route();
    
private:
    crow::SimpleApp app;
};

 
