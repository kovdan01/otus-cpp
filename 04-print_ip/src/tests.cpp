#include "print_ip.h"

#include <gtest/gtest.h>

#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <iostream>

TEST(PrintIp, Integer)
{
    testing::internal::CaptureStdout();

    my::print_ip(char(-1));
    std::cout << std::endl;
    my::print_ip(short(0));
    std::cout << std::endl;
    my::print_ip(int(2130706433));
    std::cout << std::endl;
    my::print_ip(long(8875824491850138409));
    std::cout << std::endl;

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("255\n"
              "0.0\n"
              "127.0.0.1\n"
              "123.45.67.89.101.112.131.41\n", output);
}

TEST(PrintIp, Vector)
{
    testing::internal::CaptureStdout();

    my::print_ip(std::vector{"print", "from", "vector"});
    std::cout << std::endl;
    my::print_ip(std::vector{1, 2, 3, 4});
    std::cout << std::endl;
    my::print_ip(std::vector{1});
    std::cout << std::endl;
    my::print_ip(std::vector<int>{});
    std::cout << std::endl;

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("print.from.vector\n"
              "1.2.3.4\n"
              "1\n"
              "\n", output);
}

TEST(PrintIp, List)
{
    testing::internal::CaptureStdout();

    my::print_ip(std::list{"print", "from", "list"});
    std::cout << std::endl;
    my::print_ip(std::list{1, 2, 3, 4});
    std::cout << std::endl;
    my::print_ip(std::list{1});
    std::cout << std::endl;
    my::print_ip(std::list<int>{});
    std::cout << std::endl;

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("print.from.list\n"
              "1.2.3.4\n"
              "1\n"
              "\n", output);
}

TEST(PrintIp, String)
{
    testing::internal::CaptureStdout();

    my::print_ip(std::string{"trololo"});
    std::cout << std::endl;

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("trololo\n", output);
}

TEST(PrintIp, Tuple)
{
    testing::internal::CaptureStdout();

    my::print_ip(std::make_tuple("print", "from", "tuple"));
    std::cout << std::endl;
    my::print_ip(std::make_tuple(1, 2, 3, 4));
    std::cout << std::endl;
    my::print_ip(std::make_tuple(1));
    std::cout << std::endl;
    my::print_ip(std::make_tuple());
    std::cout << std::endl;

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("print.from.tuple\n"
              "1.2.3.4\n"
              "1\n"
              "\n", output);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
