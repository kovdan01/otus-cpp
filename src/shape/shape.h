#ifndef SHAPE_H
#define SHAPE_H

#include "../render/renderable.h"
#include "../utils/color.h"

#include <memory>

namespace my
{

/**
 * @brief The IShape interface
 */
class IShape : public IRenderable
{
public:
    using Ptr = std::unique_ptr<IShape>;

    virtual ~IShape() = default;

    /**
     * @brief Creates a copy of current shape
     * @return pointer to the clone
     */
    virtual Ptr clone() = 0;

    /**
     * @brief Color setter
     * @param[in] color Color to be set
     */
    void color(Color color)
    {
        m_color = color;
    }

    /**
     * @brief Color getter
     * @return Current shape color
     */
    Color color() const
    {
        return m_color;
    }

private:
    Color m_color;
};

} // namespace my

#endif // SHAPE_H
