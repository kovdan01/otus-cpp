#include "controller.h"

#include <boost/program_options.hpp>

#include <iostream>

int main(int argc, char* argv[]) try
{
    std::ios::sync_with_stdio(false);

    namespace po = boost::program_options;
    po::options_description opt_desc("Allowed options");
    opt_desc.add_options()
        ("help",                                       "Print this message")
        ("src",  po::value<std::string>()->required(), "Path to source file")
        ("mnum", po::value<std::size_t>()->required(), "Map thread workers count")
        ("rnum", po::value<std::size_t>()->required(), "Reduce thread workers count")
    ;
    po::positional_options_description pos_opt_desc;
    pos_opt_desc.add("src",  1);
    pos_opt_desc.add("mnum", 1);
    pos_opt_desc.add("rnum", 1);

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
            return 0;
        }
        po::notify(var_map);
    }
    catch (const po::error& error)
    {
        std::cerr << "Error while parsing command-line arguments: "
                  << error.what() << "\nPlease use --help to see help message\n";
        return 1;
    }

    std::size_t mnum = var_map["mnum"].as<std::size_t>();
    if (mnum == 0)
    {
        std::cerr << "Map thread workers count must be at least 1\n";
        return 1;
    }
    std::size_t rnum = var_map["rnum"].as<std::size_t>();
    if (rnum == 0)
    {
        std::cerr << "Reduce thread workers count must be at least 1\n";
        return 1;
    }
    std::string src = var_map["src"].as<std::string>();

    my::Controller controller(mnum, rnum);
    std::size_t ans = controller.work(src);
    if (ans == std::size_t(-1))
        std::cout << "Cannot detect minimum prefix" << std::endl;
    else
        std::cout << "Minimum prefix " << ans << std::endl;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
