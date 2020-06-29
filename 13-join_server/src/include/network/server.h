#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <boost/asio.hpp>

#include <mutex>

namespace my
{

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_service& io_context, short port);

private:
    void do_accept();

    tcp::acceptor m_acceptor;
};

} // namespace my

#endif // NETWORK_SERVER_H
