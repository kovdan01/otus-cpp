#include "network_session.h"
#include "controller.h"

namespace my
{

Session::Session(std::size_t bulk, tcp::socket socket)
    : m_bulk(bulk)
    , m_socket(std::move(socket))
{
}

void Session::start()
{
    do_read();
}

void Session::do_read()
{
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_buffer, BUFFER_SIZE),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                ControllerSingletone::get_instance(m_bulk)->receive(m_buffer, length);
                do_write(length);
            }
        });
}

void Session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_buffer, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                do_read();
            }
        });
}

} // namespace my
