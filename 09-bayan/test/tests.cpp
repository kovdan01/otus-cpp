#include "mask_filesystem_traverser.h"
#include "duplicate_finder.h"
#include "hash/crc32.h"

#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <regex>
#include <algorithm>

namespace fs = std::filesystem;

struct FilesystemTraverserTestParam
    : public ::testing::TestWithParam
    <
        std::tuple
        <
            std::vector<std::string>,
            std::vector<std::string>,
            std::vector<std::string>,
            std::vector<std::string>,
            std::size_t,
            bool,
            bool,
            std::vector<fs::path>,
            std::size_t,
            std::vector<std::vector<fs::path>>
        >
    >
{
    std::vector<std::string> root_directories;
    std::vector<std::string> exclude_directories;
    std::vector<std::string> file_masks_include;
    std::vector<std::string> file_masks_exclude;
    std::size_t min_file_size;
    bool recursive;
    bool case_sensitive;
    std::vector<fs::path> filtered;

    std::size_t block_size;
    std::vector<std::vector<fs::path>> duplicates;
};

TEST_P(FilesystemTraverserTestParam, FilterAndFindDuplicates)
{
    const std::vector<std::string>& root_directories     = std::get<0>(GetParam());
    const std::vector<std::string>& exclude_directories  = std::get<1>(GetParam());
    const std::vector<std::string>& file_masks_include   = std::get<2>(GetParam());
    const std::vector<std::string>& file_masks_exclude   = std::get<3>(GetParam());
    std::size_t min_file_size                            = std::get<4>(GetParam());
    bool recursive                                       = std::get<5>(GetParam());
    bool case_sensitive                                  = std::get<6>(GetParam());
    const std::vector<fs::path>& filtered                = std::get<7>(GetParam());
    std::size_t block_size                               = std::get<8>(GetParam());
    const std::vector<std::vector<fs::path>>& duplicates = std::get<9>(GetParam());

    my::MaskFilesystemTraverser filesystem_traverser(recursive);

    for (const std::string& root_directory : root_directories)
        filesystem_traverser.add_root_directory(fs::path(root_directory));
    for (const std::string& exclude_directory : exclude_directories)
        filesystem_traverser.add_exclude_directory(fs::path(exclude_directory));

    std::regex::flag_type re_flags = (case_sensitive ? std::regex_constants::ECMAScript : std::regex_constants::icase);
    for (const std::string& mask_include : file_masks_include)
        filesystem_traverser.add_file_mask_include(std::regex(mask_include, re_flags));
    for (const std::string& mask_exclude : file_masks_exclude)
        filesystem_traverser.add_file_mask_exclude(std::regex(mask_exclude, re_flags));

    filesystem_traverser.set_min_file_size(min_file_size);

    std::vector<fs::path> files = filesystem_traverser.get_files_list();
    std::sort(files.begin(), files.end());
    EXPECT_EQ(files, filtered);

    my::DuplicateFinder<my::Crc32> duplicate_finder(block_size);
    duplicate_finder.add_filesystem_traverser(&filesystem_traverser);
    std::vector<std::vector<fs::path>> equal = duplicate_finder.get_duplicates();
    for (auto& group : equal)
        std::sort(group.begin(), group.end());
    std::sort(equal.begin(), equal.end());
    EXPECT_EQ(equal, duplicates);
}

INSTANTIATE_TEST_CASE_P
(
    FilesystemTraverser,
    FilesystemTraverserTestParam,
    ::testing::Values
    (
        std::make_tuple
        (
            std::vector<std::string>{"a"},
            std::vector<std::string>{},
            std::vector<std::string>{},
            std::vector<std::string>{},
            1,
            false,
            false,
            std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/2.LOG")},
            4096,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/2.LOG")}
            }
        ),
        std::make_tuple
        (
            std::vector<std::string>{"a"},
            std::vector<std::string>{},
            std::vector<std::string>{},
            std::vector<std::string>{},
            1,
            true,
            false,
            std::vector<fs::path>
            {
                fs::path("a/1.log"),
                fs::path("a/2.LOG"),
                fs::path("a/b/3.log"),
                fs::path("a/b/4.in"),
                fs::path("a/c/5.in"),
                fs::path("a/c/6.in")
            },
            4096,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/2.LOG"), fs::path("a/c/5.in")},
                std::vector<fs::path>{fs::path("a/b/3.log"), fs::path("a/c/6.in")}
            }
        ),
        std::make_tuple
        (
            std::vector<std::string>{"a"},
            std::vector<std::string>{"a/c"},
            std::vector<std::string>{},
            std::vector<std::string>{},
            1,
            true,
            false,
            std::vector<fs::path>
            {
                fs::path("a/1.log"),
                fs::path("a/2.LOG"),
                fs::path("a/b/3.log"),
                fs::path("a/b/4.in")
            },
            4096,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/2.LOG")}
            }
        ),
        std::make_tuple
        (
            std::vector<std::string>{"a", "a/c"},
            std::vector<std::string>{},
            std::vector<std::string>{},
            std::vector<std::string>{},
            1,
            false,
            false,
            std::vector<fs::path>
            {
                fs::path("a/1.log"),
                fs::path("a/2.LOG"),
                fs::path("a/c/5.in"),
                fs::path("a/c/6.in")
            },
            4096,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/2.LOG"), fs::path("a/c/5.in")}
            }
        ),
        std::make_tuple
        (
            std::vector<std::string>{"a"},
            std::vector<std::string>{},
            std::vector<std::string>{".*\\.log"},
            std::vector<std::string>{},
            1,
            true,
            false,
            std::vector<fs::path>
            {
                fs::path("a/1.log"),
                fs::path("a/2.LOG"),
                fs::path("a/b/3.log"),
            },
            4096,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/2.LOG")}
            }
        ),
        std::make_tuple
        (
            std::vector<std::string>{"a"},
            std::vector<std::string>{},
            std::vector<std::string>{},
            std::vector<std::string>{".*\\.log"},
            1,
            true,
            true,
            std::vector<fs::path>
            {
                fs::path("a/2.LOG"),
                fs::path("a/b/4.in"),
                fs::path("a/c/5.in"),
                fs::path("a/c/6.in")
            },
            4096,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/2.LOG"), fs::path("a/c/5.in")}
            }
        ),
        std::make_tuple
        (
            std::vector<std::string>{"a"},
            std::vector<std::string>{},
            std::vector<std::string>{},
            std::vector<std::string>{},
            4,
            true,
            true,
            std::vector<fs::path>
            {
                fs::path("a/b/3.log"),
                fs::path("a/c/6.in")
            },
            1,
            std::vector<std::vector<fs::path>>
            {
                std::vector<fs::path>{fs::path("a/b/3.log"), fs::path("a/c/6.in")}
            }
        )
    )
);

int main(int argc, char* argv[])
{
    std::vector<fs::path>{fs::path("a/1.log"), fs::path("a/1.LOG")};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
