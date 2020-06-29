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
    Session(tcp::socket socket);
    void start();

private:
    void do_read();
    void do_write(const std::string& str);

    static constexpr std::size_t BUFFER_SIZE = 1024;
    tcp::socket m_socket;
    char m_buffer[BUFFER_SIZE];
    std::string m_temp_str;
};

} // namespace my

#endif // NETWORK_SESSION_H
