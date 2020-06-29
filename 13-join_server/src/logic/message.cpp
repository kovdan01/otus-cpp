#include "logic/message.h"

#include <utility>

namespace my
{

Message::Message(Code code, std::string text)
    : m_code(code)
    , m_text(std::move(text))
{
}

Message::Code Message::code() const
{
    return m_code;
}

const std::string& Message::text() const
{
    return m_text;
}

} // namespace my
