#pragma once

#include "Header.h"
#include <memory>
#include "base/FASIOType.h"
namespace fasio
{
class TCPSession;
class TCPSessionFactory
{
public:
	virtual std::shared_ptr<TCPSession> createSession(std::shared_ptr<asio::ip::tcp::socket>) = 0;
	virtual asio::io_context& io_context() = 0;
    virtual uint8 type() = 0;
};


}
