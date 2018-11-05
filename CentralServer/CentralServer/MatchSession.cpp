//
//  MatchSession.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/1.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "MatchSession.hpp"
#include <Net/Util/NetPacket.hpp>
#include "CSKernel.hpp"

using namespace fasio;

void MatchSession::onClose()
{
    CSKernel::instance().removeService(logicID());
}
