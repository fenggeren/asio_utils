#include "TCPListener.h"
#include "TCPSessionFactory.h"
#include "TCPSession.hpp"

namespace fasio
{

TCPListener::TCPListener(std::shared_ptr<TCPSessionFactory> sessionFactory)
:acceptor_(sessionFactory->io_context())
, sessionFactory_(sessionFactory)
{
    acceptor_.setNewConnectionCallback(std::bind(&TCPListener::newConnection, this, std::placeholders::_1));
}

void TCPListener::listen(uint16 port, bool ipv6)
{
    acceptor_.listen(port, ipv6);
}

void TCPListener::newConnection(std::shared_ptr<tcp::socket> sock)
{
    // auto endpoint = sock->remote_endpoint();
    // char buf[64] = {0};
    // snprintf(buf, sizeof(buf), "%s:%d", endpoint.address().to_string().data(), 
    //                                     endpoint.port());

    auto session = sessionFactory_->createSession(sock);
    session->setType(sessionFactory_->type());
    newSessionCallback_(session);
    session->connectEstablished();
}

}
