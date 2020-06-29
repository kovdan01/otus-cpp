#include "dummy_command_processor.h"
#include "command_storage.h"
#include "console_reader.h"
#include "console_writer.h"
#include "file_writer.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <stdexcept>

struct Context
{
    std::size_t block_size;
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
        ("size", po::value<std::size_t>()->required(), "Command block size (integer, minimum 1)")
    ;
    po::positional_options_description pos_opt_desc;
    pos_opt_desc.add("size", 1);

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

    std::size_t block_size = var_map["size"].as<std::size_t>();
    if (block_size == 0)
    {
        std::cerr << "Block size must be at least 1\n";
        throw Error{};
    }

    return Context{block_size};
}

int main(int argc, char* argv[])
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

    my::ConsoleWriter console_writer;
    my::FileWriter file_writer("bulk", ".log");
    my::DummyCommandProcessor bulk_command_processor;
    my::CommandStorage command_storage;
    my::ConsoleReader console_reader(context.block_size);

    bulk_command_processor.add_writer(&console_writer);
    bulk_command_processor.add_writer(&file_writer);
    command_storage.add_processor(&bulk_command_processor);
    console_reader.add_storage(&command_storage);

    console_reader.read();
}
