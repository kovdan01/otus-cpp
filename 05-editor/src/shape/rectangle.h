#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "../utils/point.h"

#include <memory>

namespace my
{

/**
 * @brief The Rectangle class
 * Represents a rectangle on a plane
 */
class Rectangle : public IShape
{
public:
    /**
     * @brief Constructor from the corner point, width and height
     * @param[in] corner Upper-left corner
     * @param[in] width Width
     * @param[in] height Height
     */
    Rectangle(Point corner, int width, int height)
        : m_corner(corner)
        , m_width(width)
        , m_height(height)
    {
    }

    Rectangle(const Rectangle&) = default;
    Rectangle& operator=(const Rectangle&) = default;
    Rectangle(Rectangle&&) = default;
    Rectangle& operator=(Rectangle&&) = default;

    ~Rectangle() override = default;

    void render(Canvas& canvas) override
    {
        canvas << "Rectangle{ " << color() << ", " << m_corner << ", " << m_width << ", " << m_height << " }";
    }

    Ptr clone() override
    {
        return std::make_unique<Rectangle>(m_corner, m_width, m_height);
    }

private:
    Point m_corner;
    int m_width, m_height;
};

} // namespace my

#endif // RECTANGLE_H
