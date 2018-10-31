
#pragma once

#include "TCPAcceptor.hpp"
#include "Header.h"
#include "base/NetBase.h"
namespace fasio
{
class TCPSessionFactory;
class TCPSession;
    
    
class TCPListener
{
public:
    TCPListener(std::shared_ptr<TCPSessionFactory> sessionFactory);

    void listen(uint16 port, bool ipv6);

    void setNewSessionCallback(const std::function<void(std::shared_ptr<TCPSession>)>& cb)
    { newSessionCallback_ = cb; }
private: 

    void newConnection(std::shared_ptr<tcp::socket>);

private:
    TCPAcceptor acceptor_;

    std::function<void(std::shared_ptr<TCPSession>)> newSessionCallback_;
    std::shared_ptr<TCPSessionFactory> sessionFactory_;
};


}
