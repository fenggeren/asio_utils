//
//  ConvFunctional.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/15.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <string>
#include <sstream>

template<typename T>
std::string jointContainer(const T& cont)
{
    std::stringstream ss;
    std::string res;
    for(auto& ele : cont)
    {
        ss << ele;
        ss << ", ";
    }
    return ss.str();
}


