#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

#include "network/raw_data_handler.h"

#include <boost/asio.hpp>

#include <memory>
#include <mutex>

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

    static constexpr std::size_t BUFFER_SIZE = 1024;
    tcp::socket m_socket;
    char m_buffer[BUFFER_SIZE];

    RawDataHandler m_controller;
};

} // namespace my

#endif // NETWORK_SESSION_H
