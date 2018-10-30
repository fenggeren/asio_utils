#pragma once

#include "Header.h"
#include <memory>
namespace fasio
{
class TCPSession;
class TCPSessionFactory
{
public:
	virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket>) = 0;
	virtual asio::io_context& io_context() = 0;
};


}