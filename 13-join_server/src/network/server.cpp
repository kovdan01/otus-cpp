#include "network/server.h"
#include "network/session.h"

namespace my
{

Server::Server(boost::asio::io_service& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    do_accept();
}

void Server::do_accept()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket))->start();
            }

            do_accept();
        });
}

} // namespace my
