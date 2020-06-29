#include "network/server.h"

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <stdexcept>

struct Context
{
    short port;
};

struct Help : public std::exception
{
};

struct Error : public std::exception
{
};

Context parse_command_options(int argc, char* argv[])
{
    namespace po = boost::program_options;
    po::options_description opt_desc("Allowed options");
    opt_desc.add_options()
        ("help",                                       "Print this message")
        ("port", po::value<short>()->required(),       "Server port")
    ;
    po::positional_options_description pos_opt_desc;
    pos_opt_desc.add("port", 1);

    po::variables_map var_map;
    try
    {
        auto parsed = po::command_line_parser(argc, argv)
            .options(opt_desc)
            .positional(pos_opt_desc)
            .run();
        po::store(parsed, var_map);
        if (var_map.count("help") != 0)
        {
            std::cout << opt_desc << "\n";
            throw Help{};
        }
        po::notify(var_map);
    }
    catch (const po::error& error)
    {
        std::cerr << "Error while parsing command-line arguments: "
                  << error.what() << "\nPlease use --help to see help message\n";
        throw Error{};
    }

    short port = var_map["port"].as<short>();

    return Context{port};
}

int main(int argc, char* argv[]) try
{
    std::ios::sync_with_stdio(false);

    Context context;
    try
    {
        context = parse_command_options(argc, argv);
    }
    catch (const Help&)
    {
        return 0;
    }
    catch (const Error&)
    {
        return 1;
    }

    boost::asio::io_service io_context;

    my::Server server(io_context, context.port);

    io_context.run();
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
