#include "dummy_command_processor.h"
#include "command_storage.h"
#include "console_reader.h"
#include "console_writer.h"

#include "async.h"

#include <gtest/gtest.h>

#include <sstream>
#include <tuple>
#include <string>

struct BulkTestParam
    : public ::testing::TestWithParam
    <
        std::tuple
        <
            std::string,
            std::string,
            std::size_t
        >
    >
{
    std::string input;
    std::string output;
    std::size_t block_size;
};

TEST_P(BulkTestParam, TestOutput)
{
    const std::string& str     = std::get<0>(GetParam());
    const std::string& correct = std::get<1>(GetParam());
    std::size_t block_size     = std::get<2>(GetParam());

    std::istringstream input(str);

    testing::internal::CaptureStdout();

    {
        my::ConsoleWriter console_writer;
        my::DummyCommandProcessor bulk_command_processor;
        my::CommandStorage command_storage;
        my::ConsoleReader console_reader(block_size, input);

        bulk_command_processor.add_writer(&console_writer);
        command_storage.add_processor(&bulk_command_processor);
        console_reader.add_storage(&command_storage);

        console_reader.read();
    }

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(correct, output);
}

INSTANTIATE_TEST_CASE_P
(
    Bulk,
    BulkTestParam,
    ::testing::Values
    (
        std::make_tuple
        (
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5",

            "bulk: cmd1, cmd2, cmd3\n"
            "bulk: cmd4, cmd5\n",

             3
        ),
        std::make_tuple
        (
            "cmd1"    "\n"
            "cmd2"    "\n"
            "cmd3"    "\n"
            "cmd4"    "\n"
            "cmd5"    "\n"
            "{"       "\n"
            "cmd6"    "\n"
            "cmd7"    "\n"
            "{"       "\n"
            "cmd8"    "\n"
            "cmd9"    "\n"
            "cmd10"   "\n"
            "}"       "\n"
            "cmd11"   "\n"
            "{"       "\n"
            "}"       "\n"
            "}"       "\n"
            "cmd12",

            "bulk: cmd1, cmd2, cmd3, cmd4\n"
            "bulk: cmd5\n"
            "bulk: cmd6, cmd7, cmd8, cmd9, cmd10, cmd11\n"
            "bulk: cmd12\n",

            4
        ),
        std::make_tuple
        (
            "cmd1"    "\n"
            "cmd2"    "\n"
            "{"       "\n"
            "cmd3"    "\n"
            "cmd4"    "\n"
            "cmd5"    "\n"
            "cmd6",

            "bulk: cmd1\n"
            "bulk: cmd2\n",

            1
        ),
        std::make_tuple
        (
            "{"       "\n"
            "cmd1"    "\n"
            "cmd2"    "\n"
            "cmd3"    "\n"
            "{"       "\n"
            "cmd4"    "\n"
            "cmd5"    "\n"
            "}"       "\n"
            "cmd6",

            "",

            1
        )
    )
);

TEST(Bulk, Async)
{
    testing::internal::CaptureStdout();

    std::size_t bulk = 5;
    async::handle_t h = async::connect(bulk);
    async::handle_t h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "", 0);
    async::receive(h, "\n", 1);
    async::receive(h, "2\n3\n4\n5\n6\n{\na\n", 14);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("bulk: 1, 2, 3, 4, 5\n"
              "bulk: 6\n"
              "bulk: a, b, c, d\n"
              "bulk: 89\n"
              "bulk: 1\n", output);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
