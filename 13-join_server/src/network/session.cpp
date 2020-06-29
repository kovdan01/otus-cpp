#include "network/session.h"
#include "logic/command.h"
#include "utils/thread_pool.h"

#include <iostream>

namespace my
{

Session::Session(tcp::socket socket)
    : m_socket(std::move(socket))
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
            if (ec)
                return;

            std::string str(m_buffer, length);
            std::size_t pos = str.rfind('\n');

            if (pos == std::string::npos)
            {
                m_temp_str += str;
                do_read();
                return;
            }

            auto input = std::make_shared<std::istringstream>(m_temp_str + str.substr(0, pos));
            m_temp_str = std::string(str.data() + pos + 1, str.size() - pos - 1);

            auto job = [this, input, self]()
            {
                std::string line;
                std::string ans;
                while (std::getline(*input, line))
                    ans += my::run_command(line) + "\n";

                do_write(ans);
            };

            progschj::ThreadPool::get_instance()->enqueue(job);
        });
}

void Session::do_write(const std::string& str)
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(str.c_str(), str.size()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                do_read();
            }
        });
}

} // namespace my
