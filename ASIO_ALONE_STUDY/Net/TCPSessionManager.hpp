//
//  NetSessionManager.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/10/30.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "TCPSession.hpp"


namespace fasio
{

class TCPSessionFactory;
class TCPSession;
class TCPAcceptor;
class TCPListener;

struct Listener 
{
	std::shared_ptr<TCPSessionFactory> factory_;
	std::shared_ptr<TCPAcceptor> acceptor_;
};

class TCPSessionManager
{
public:
    TCPSessionManager() {}
    virtual ~TCPSessionManager() {}
 
public:
	void createListener(int port, bool ipv6, std::shared_ptr<TCPSessionFactory> factory);

public:

    void addSession(TCPSessionPtr session);
    
	void removeSession(int32 uuid);
	TCPSessionPtr getSession(int32 uuid);

	// stype:  服务器类型
	void sendMsgToSession(int32 uuid, const void* data, int len, int msgID, uint8 stype = 0);

public:

	void newSession(std::shared_ptr<TCPSession> session);
	
private:
    void removeSessionPtr(const TCPSessionPtr& session);
    
protected:
    std::unordered_map<uint16 ,std::shared_ptr<TCPListener>> listeners_;
    std::unordered_map<int32, TCPSessionPtr> sessionMap_;
};



}



