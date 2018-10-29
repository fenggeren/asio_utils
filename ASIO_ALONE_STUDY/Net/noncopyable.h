//
//  noncopyable.h
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/21.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

namespace fasio
{

    
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    noncopyable(noncopyable&&);
    const noncopyable& operator=(const noncopyable&);
    const noncopyable& operator=(noncopyable&&);
 
};


}
