//
//  Header.h
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/9/20.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#ifdef ASIO_ONLY

#include "asio.hpp"

#else

#include <boost/asio.hpp>
using namespace boost;

#if BOOST_ASIO_VERSION < 101200

#define  io_context io_service

#endif

//using namespace asio;

#endif


