#include <boost/asio.hpp>

#include <mutex>

namespace my
{

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_service& io_context, short port, std::size_t bulk);

private:
    void do_accept();

    tcp::acceptor m_acceptor;
    std::size_t m_bulk;
};

} // namespace my
