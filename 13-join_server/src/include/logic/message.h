#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

namespace my
{

/**
 * @brief The Message class
 * Contains message code (OK, ERROR) and message text
 * Used to transport data between different entities
 */
class Message
{
public:
    enum class Code
    {
        OK,
        ERROR,
    };

    Message(Code code, std::string text);

    Message() = delete;

    Message(const Message&) = default;
    Message& operator=(const Message&) = default;
    Message(Message&&) = default;
    Message& operator=(Message&&) = default;

    [[nodiscard]] Code code() const;
    [[nodiscard]] const std::string& text() const;

private:
    Code m_code;
    std::string m_text;
};

} // namespace my

#endif // MESSAGE_H
