#include "mask_filesystem_traverser.h"
#include "duplicate_finder.h"
#include "hash/crc32.h"
#include "hash/crc16.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

template <typename Hash>
void job(std::size_t block_size, my::IFilesystemTraverser* filesystem_traverser)
{
    my::DuplicateFinder<Hash> duplicate_finder(block_size);
    duplicate_finder.add_filesystem_traverser(filesystem_traverser);

    std::vector<std::vector<fs::path>> equal = duplicate_finder.get_duplicates();
    for (const std::vector<fs::path>& group : equal)
    {
        for (const fs::path& path : group)
            std::cout << path << '\n';
        std::cout << '\n';
    }
}

int main(int argc, char* argv[]) try
{
    namespace po = boost::program_options;
    po::options_description opt_desc("Allowed options");
    bool recursive = false;
    bool case_sensitive = false;
    opt_desc.add_options()
        ("help",                                                                            "Print this message")
        ("size",           po::value<std::size_t>()->required()->default_value(4096),       "Block size (in bytes) used to compare files (at least 1)")
        ("hash",           po::value<std::string>()->required()->default_value("crc32"),    "Hash algorithm used to compare byte blocks, one of 'crc32', 'crc16'")
        ("min_file_size",  po::value<std::size_t>()->required()->default_value(1),          "Minimum file size to compare")
        ("root_dir",       po::value<std::vector<std::string>>()->required()->multitoken(), "Directories to search duplicates into")
        ("exclude_dir",    po::value<std::vector<std::string>>()->multitoken(),             "Direcroties to exclude from search")
        ("mask_include",   po::value<std::vector<std::string>>()->multitoken(),             "Include only files corresponding to these masks in search")
        ("mask_exclude",   po::value<std::vector<std::string>>()->multitoken(),             "Exclude files corresponding to these masks from search")
        ("recursive",      po::bool_switch(&recursive),                                     "Use this option to enable recursive subdirectory scanning")
        ("case_sensitive", po::bool_switch(&case_sensitive),                                "Use this option to make file masks case sensitive")
    ;

    po::variables_map var_map;
    try
    {
        auto parsed = po::command_line_parser(argc, argv)
            .options(opt_desc)
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

    std::size_t block_size = var_map["size"].as<std::size_t>();
    if (block_size == 0)
    {
        std::cerr << "Block size must be at least 1\n";
        return 1;
    }
    std::size_t min_file_size = var_map["min_file_size"].as<std::size_t>();

    std::vector<std::string> root_directories    = var_map["root_dir"].as<std::vector<std::string>>();

    std::vector<std::string> exclude_directories = var_map.count("exclude_dir") == 0
                                                   ? std::vector<std::string>()
                                                   : var_map["exclude_dir"].as<std::vector<std::string>>();

    std::vector<std::string> masks_include       = var_map.count("mask_include") == 0
                                                   ? std::vector<std::string>()
                                                   : var_map["mask_include"].as<std::vector<std::string>>();

    std::vector<std::string> masks_exclude       = var_map.count("mask_exclude") == 0
                                                   ? std::vector<std::string>()
                                                   : var_map["mask_exclude"].as<std::vector<std::string>>();

    my::MaskFilesystemTraverser filesystem_traverser(recursive);

    for (std::string& root_directory : root_directories)
        filesystem_traverser.add_root_directory(fs::path(std::move(root_directory)));
    for (std::string& exclude_directory : exclude_directories)
        filesystem_traverser.add_exclude_directory(fs::path(std::move(exclude_directory)));

    std::regex::flag_type re_flags = (case_sensitive ? std::regex_constants::ECMAScript : std::regex_constants::icase);
    for (const std::string& mask_include : masks_include)
        filesystem_traverser.add_file_mask_include(std::regex(mask_include, re_flags));
    for (const std::string& mask_exclude : masks_exclude)
        filesystem_traverser.add_file_mask_exclude(std::regex(mask_exclude, re_flags));

    filesystem_traverser.set_min_file_size(min_file_size);

    std::string hash_algo = var_map["hash"].as<std::string>();

    if (hash_algo == "crc32")
        job<my::Crc32>(block_size, &filesystem_traverser);
    else if (hash_algo == "crc16")
        job<my::Crc16>(block_size, &filesystem_traverser);
    else
        throw std::invalid_argument(std::string("Incorrect hash algorithm ") + hash_algo + ". Use --help options for more details");
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
