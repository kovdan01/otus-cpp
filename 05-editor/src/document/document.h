#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "../render/renderable.h"
#include "../shape/shape.h"
#include "../shape/circle.h"
#include "../shape/rectangle.h"
#include "../shape/segment.h"

#include <unordered_set>
#include <cassert>

namespace my
{

/**
 * @brief The Document class
 * Represents a document with shapes
 */
class Document : public IRenderable
{
public:
    using Ptr = std::unique_ptr<Document>;

    Document() = default;
    virtual ~Document() = default;

    void render(Canvas& canvas) override
    {
        for (const IShape::Ptr& shape : m_shapes)
        {
            shape->render(canvas);
            canvas << std::endl;
        }
    }

    /**
     * @brief Adds a circle to the document
     * @param[in] Circle center
     * @param[in] Circle radius
     * @return Pointer to the added circle
     */
    const IShape::Ptr& add_circle(Point center, int radius)
    {
        auto [it, b_inserted] = m_shapes.emplace(std::make_unique<Circle>(center, radius));
        assert(b_inserted);
        return *it;
    }

    /**
     * @brief Adds a rectangle to the document
     * @param[in] Rectangle upper-left corner
     * @param[in] Rectangle width
     * @param[in] Rectangle height
     * @return Pointer to the added rectangle
     */
    const IShape::Ptr& add_rectangle(Point corner, int width, int height)
    {
        auto [it, b_inserted] = m_shapes.emplace(std::make_unique<Rectangle>(corner, width, height));
        assert(b_inserted);
        return *it;
    }

    /**
     * @brief Adds a segment to the document
     * @param First segment point
     * @param Second segment point
     * @return Pointer to the added segment
     */
    const IShape::Ptr& add_segment(Point point1, Point point2)
    {
        auto [it, b_inserted] = m_shapes.emplace(std::make_unique<Segment>(point1, point2));
        assert(b_inserted);
        return *it;
    }

    /**
     * @brief Removes the given shape
     * @param[in] shape Shape to be removed
     * @return `true` if a remove had place, `false` otherwise
     */
    bool remove_shape(const IShape::Ptr& shape)
    {
        std::size_t removed = m_shapes.erase(shape);
        if (removed == 0)
            return false;
        return true;
    }

private:
    std::unordered_set<IShape::Ptr> m_shapes;
};

} // namespace my

#endif // DOCUMENT_H
