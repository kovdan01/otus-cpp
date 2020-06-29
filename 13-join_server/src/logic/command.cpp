#include "logic/command.h"
#include "logic/database.h"

#include <sstream>

namespace my
{

std::string to_string(const Message& message)
{
    switch (message.code())
    {
    case Message::Code::OK:
        return "OK";
    case Message::Code::ERROR:
        return std::string{"ERR "} + message.text();
    }
    // Suppress compiler warning "control reaches end of non-void function"
    // Should not be called unless all Message::Code enum values are covered in switch
    std::terminate();
}

std::string insert(const std::string& table, Database::Index index, const Database::Data& data)
{
    return to_string(Database::get_instance()->insert(table, index, data));
}

std::string truncate(const std::string& table)
{
    return to_string(Database::get_instance()->truncate(table));
}

std::string intersection()
{
    auto [message, inter] = Database::get_instance()->intersection("A", "B");
    switch (message.code())
    {
    case Message::Code::OK:
    {
        std::string ans;
        for (const auto& [id, value1, value2] : inter)
            ans += std::to_string(id) + "," + value1 + "," + value2 + "\n";
        ans += to_string(message);
        return ans;
    }
    case Message::Code::ERROR:
    {
        return to_string(message);
    }
    }
    // Suppress compiler warning "control reaches end of non-void function"
    // Should not be called unless all Message::Code enum values are covered in switch
    std::terminate();
}

std::string symmetric_difference()
{
    auto [message, diff] = Database::get_instance()->symmetric_difference("A", "B");
    switch (message.code())
    {
    case Message::Code::OK:
    {
        std::string ans;
        for (const auto& [id, value1, value2] : diff)
            ans += std::to_string(id) + "," + value1.value_or("") + "," + value2.value_or("") + "\n";
        ans += to_string(message);
        return ans;
    }
    case Message::Code::ERROR:
    {
        return to_string(message);
    }
    }
    // Suppress compiler warning "control reaches end of non-void function"
    // Should not be called unless all Message::Code enum values are covered in switch
    std::terminate();
}

std::string run_command(const std::string& command)
{
    std::string tag, remaining;
    std::istringstream stream(command);
    stream >> tag;
    std::string invalid_args_msg = to_string(Message{Message::Code::ERROR, std::string{"Invalid arguments for command "} + tag});
    std::string too_many_args_msg = to_string(Message{Message::Code::ERROR, std::string{"Too many arguments in command "} + command});
    if (tag == "INSERT")
    {
        std::string table;
        stream >> table;
        if (!stream)
            return invalid_args_msg;

        Database::Index id;
        stream >> id;
        if (!stream)
            return invalid_args_msg;

        Database::Data data;
        stream >> data;
        if (!stream)
            return invalid_args_msg;

        stream >> remaining;
        if (stream)
            return too_many_args_msg;

        return insert(table, id, data);
    }
    else if (tag == "TRUNCATE")
    {
        std::string table;
        stream >> table;
        if (!stream)
            return invalid_args_msg;

        stream >> remaining;
        if (stream)
            return too_many_args_msg;

        return truncate(table);
    }
    else if (tag == "INTERSECTION")
    {
        stream >> remaining;
        if (stream)
            return too_many_args_msg;

        return intersection();
    }
    else if (tag == "SYMMETRIC_DIFFERENCE")
    {
        stream >> remaining;
        if (stream)
            return too_many_args_msg;

        return symmetric_difference();
    }
    return to_string(Message{Message::Code::ERROR, std::string{"Unknown command tag "} + tag});
}

} // namespace my
