#include "logic/database.h"
#include "logic/command.h"

#include <gtest/gtest.h>

TEST(Database, Insertion)
{
    my::Database* db = my::Database::get_instance();
    db->truncate("A");
    db->truncate("B");

    EXPECT_EQ(db->insert("A", 0, "lean").code(),       my::Message::Code::OK);
    EXPECT_EQ(db->insert("A", 0, "understand").code(), my::Message::Code::ERROR);
    EXPECT_EQ(db->insert("A", 1, "sweater").code(),    my::Message::Code::OK);
    EXPECT_EQ(db->insert("A", 2, "frank").code(),      my::Message::Code::OK);
    EXPECT_EQ(db->insert("A", 3, "violation").code(),  my::Message::Code::OK);
    EXPECT_EQ(db->insert("A", 4, "quality").code(),    my::Message::Code::OK);
    EXPECT_EQ(db->insert("A", 5, "precision").code(),  my::Message::Code::OK);

    EXPECT_EQ(db->insert("B", 3, "proposal").code(),   my::Message::Code::OK);
    EXPECT_EQ(db->insert("B", 4, "example").code(),    my::Message::Code::OK);
    EXPECT_EQ(db->insert("B", 5, "lake").code(),       my::Message::Code::OK);
    EXPECT_EQ(db->insert("B", 6, "flour").code(),      my::Message::Code::OK);
    EXPECT_EQ(db->insert("B", 7, "wonder").code(),     my::Message::Code::OK);
    EXPECT_EQ(db->insert("B", 8, "selection").code(),  my::Message::Code::OK);

    EXPECT_EQ(db->insert("C", 8, "selection").code(),  my::Message::Code::ERROR);
}

void prepare_tables()
{
    my::Database* db = my::Database::get_instance();
    db->truncate("A");
    db->truncate("B");

    db->insert("A", 0, "lean");
    db->insert("A", 1, "sweater");
    db->insert("A", 2, "frank");
    db->insert("A", 3, "violation");
    db->insert("A", 4, "quality");
    db->insert("A", 5, "precision");
    db->insert("B", 3, "proposal");
    db->insert("B", 4, "example");
    db->insert("B", 5, "lake");
    db->insert("B", 6, "flour");
    db->insert("B", 7, "wonder");
    db->insert("B", 8, "selection");
}

TEST(Database, Intersection)
{
    my::Database* db = my::Database::get_instance();

    auto [message, inter] = db->intersection("A", "B");
    EXPECT_EQ(message.code(), my::Message::Code::OK);

    std::map<int, std::pair<my::Database::Data, my::Database::Data>> correct =
    {
        {3, {"violation", "proposal"}},
        {4, {"quality", "example"}},
        {5, {"precision", "lake"}},
    };
    EXPECT_EQ(inter.size(), correct.size());

    auto inter_it = inter.begin();
    auto correct_it = correct.begin();

    for (; correct_it != correct.end(); ++inter_it, ++correct_it)
    {
        EXPECT_EQ((*inter_it).id, correct_it->first);
        EXPECT_EQ((*inter_it).first, correct_it->second.first);
        EXPECT_EQ((*inter_it).second, correct_it->second.second);
    }
    EXPECT_EQ(inter_it, inter.end());
}

TEST(Database, SymmetricDifference)
{
    prepare_tables();

    my::Database* db = my::Database::get_instance();
    auto [message, inter] = db->symmetric_difference("A", "B");
    EXPECT_EQ(message.code(), my::Message::Code::OK);

    std::map<int, std::pair<std::optional<my::Database::Data>, std::optional<my::Database::Data>>> correct =
    {
        {0, {"lean", std::nullopt}},
        {1, {"sweater", std::nullopt}},
        {2, {"frank", std::nullopt}},
        {6, {std::nullopt, "flour"}},
        {7, {std::nullopt, "wonder"}},
        {8, {std::nullopt, "selection"}},
    };
    EXPECT_EQ(inter.size(), correct.size());

    auto inter_it = inter.begin();
    auto correct_it = correct.begin();

    for (; correct_it != correct.end(); ++inter_it, ++correct_it)
    {
        EXPECT_EQ((*inter_it).id, correct_it->first);
        EXPECT_EQ((*inter_it).first, correct_it->second.first);
        EXPECT_EQ((*inter_it).second, correct_it->second.second);
    }
    EXPECT_EQ(inter_it, inter.end());
}

TEST(Command, Insertion)
{
    my::Database* db = my::Database::get_instance();
    db->truncate("A");
    db->truncate("B");

    EXPECT_EQ(my::run_command("INSERT A 0 lean"),       "OK");
    EXPECT_EQ(my::run_command("INSERT A 0 understand"), "ERR Duplicate 0");
    EXPECT_EQ(my::run_command("INSERT A 1 sweater"),    "OK");
    EXPECT_EQ(my::run_command("INSERT A 2 frank"),      "OK");
    EXPECT_EQ(my::run_command("INSERT A 3 violation"),  "OK");
    EXPECT_EQ(my::run_command("INSERT A 4 quality"),    "OK");
    EXPECT_EQ(my::run_command("INSERT A 5 precision"),  "OK");

    EXPECT_EQ(my::run_command("INSERT B 3 proposal"),   "OK");
    EXPECT_EQ(my::run_command("INSERT B 4 example"),    "OK");
    EXPECT_EQ(my::run_command("INSERT B 5 lake"),       "OK");
    EXPECT_EQ(my::run_command("INSERT B 6 flour"),      "OK");
    EXPECT_EQ(my::run_command("INSERT B 7 wonder"),     "OK");
    EXPECT_EQ(my::run_command("INSERT B 8 selection"),  "OK");

    EXPECT_EQ(my::run_command("INSERT C 8 selection"),  "ERR No such table C");
}

TEST(Command, Intersection)
{
    prepare_tables();

    EXPECT_EQ(my::run_command("INTERSECTION"),
              "3,violation,proposal\n"
              "4,quality,example\n"
              "5,precision,lake\n"
              "OK");
}

TEST(Command, SymmetricDifference)
{
    prepare_tables();

    EXPECT_EQ(my::run_command("SYMMETRIC_DIFFERENCE"),
              "0,lean,\n"
              "1,sweater,\n"
              "2,frank,\n"
              "6,,flour\n"
              "7,,wonder\n"
              "8,,selection\n"
              "OK");
}

TEST(Command, Invalid)
{
    EXPECT_EQ(my::run_command("INVALID"),
              "ERR Unknown command tag INVALID");
    EXPECT_EQ(my::run_command("INSERT A B lean"),
              "ERR Invalid arguments for command INSERT");
    EXPECT_EQ(my::run_command("INSERT A 0 lean extra"),
              "ERR Too many arguments in command INSERT A 0 lean extra");
    EXPECT_EQ(my::run_command("TRUNCATE C"),
              "ERR No such table C");
    EXPECT_EQ(my::run_command("TRUNCATE A extra"),
              "ERR Too many arguments in command TRUNCATE A extra");
    EXPECT_EQ(my::run_command("INTERSECTION extra"),
              "ERR Too many arguments in command INTERSECTION extra");
    EXPECT_EQ(my::run_command("SYMMETRIC_DIFFERENCE extra"),
              "ERR Too many arguments in command SYMMETRIC_DIFFERENCE extra");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
