#include "network/server.h"
#include "network/session.h"

namespace my
{

Server::Server(boost::asio::io_service& io_context, short port, std::size_t bulk)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    , m_bulk(bulk)
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
                std::make_shared<Session>(m_bulk, std::move(socket))->start();
            }

            do_accept();
        });
}

} // namespace my
