#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <ostream>

namespace my
{

/**
 * @brief The Color struct
 * Represents RGB color with additional transparency parameter
 */
struct Color
{
    std::uint8_t r = 0; ///< red 0-255
    std::uint8_t g = 0; ///< green 0-255
    std::uint8_t b = 0; ///< blue 0-255
    std::uint8_t a = 0; ///< alpha (transparency) 0-255
};

inline std::ostream& operator<<(std::ostream& stream, Color color)
{
    stream << "Color{ "
           << static_cast<int>(color.r) << ", "
           << static_cast<int>(color.g) << ", "
           << static_cast<int>(color.b) << ", "
           << static_cast<int>(color.a) << " }";
    return stream;
}

} // namespace my

#endif // COLOR_H
