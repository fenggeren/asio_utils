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
#include <list>
#include <google/protobuf/message.h>
#include "TCPSession.hpp"


namespace fasio
{

class TCPSessionFactory;
class TCPSession;
class TCPAcceptor;
class TCPListener;
class NetPacket;
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
    
    virtual std::shared_ptr<ClientSession>
    createConnector(uint8 type, asio::io_context& io,
                                 const std::string& ip, uint16 port);
public:

    void addSession(TCPSessionPtr session);
	void removeSession(int32 uuid);
	TCPSessionPtr getSession(int32 uuid);
    
    void addClientSession(TCPSessionPtr session);
    void removeClientSession(int32 logicid);
    TCPSessionPtr getClientSession(int32 logicid);
    
	// stype:  服务器类型
	void sendMsgToSession(int32 uuid, const void* data, int len,
                          int msgID, uint8 stype = 0);
    void sendMsgToSession(int32 uuid, google::protobuf::Message& msg,
                          int msgID, uint8 stype = 0);
    void sendMsgToSession(int32 uuid, const std::string& msg,
                          int msgID, uint8 stype = 0);
    void sendMsgToSession(int32 uuid, std::shared_ptr<NetPacket> packet,
                          uint8 stype = 0);
    void sendMsgToSession(TCPSessionPtr session, const void* data, int len,
                          int msgID, uint8 stype = 0);
    void sendMsgToSession(TCPSessionPtr session, google::protobuf::Message& msg,
                          int msgID, uint8 stype = 0);
    void sendMsgToSession(TCPSessionPtr session, const std::string& msg,
                          int msgID, uint8 stype = 0);
    void sendMsgToSession(TCPSessionPtr session, std::shared_ptr<NetPacket> packet,
                          uint8 stype = 0);
    
    void sendMsgToSession(TCPSessionPtr session, const void* data, int len, uint8 stype = 0);
public:

	void newSession(std::shared_ptr<TCPSession> session);
	
public:
    
    virtual std::shared_ptr<ClientSession> createConnectorSession(uint8 type);
    
private:
    void removeSessionPtr(const TCPSessionPtr& session); 
protected:
    std::unordered_map<uint16 ,std::shared_ptr<TCPListener>> listeners_;
    std::unordered_map<int32, TCPSessionPtr> sessionMap_;
    std::list<TCPSessionPtr> clientSessions_;
};



}



