#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

#include <boost/asio.hpp>

#include <memory>

namespace my
{

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(std::size_t bulk, tcp::socket socket);
    void start();

private:
    void do_read();
    void do_write(std::size_t length);

    static constexpr std::size_t BUFFER_SIZE = 2;
    const std::size_t m_bulk;
    tcp::socket m_socket;
    char m_buffer[BUFFER_SIZE];
};

} // namespace my

#endif // NETWORK_SESSION_H
