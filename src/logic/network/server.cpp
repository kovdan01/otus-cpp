#include "network/session.h"
#include "network/raw_data_handler.h"

namespace my
{

Session::Session(std::size_t bulk, tcp::socket socket)
    : m_socket(std::move(socket))
    , m_controller(bulk)
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
                m_controller.receive(m_buffer, length);
                do_read();
            }
        });
}

} // namespace my
