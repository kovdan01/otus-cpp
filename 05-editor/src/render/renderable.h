#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <ostream>

namespace my
{

using Canvas = std::ostream;

class IRenderable
{
public:
    /**
     * @brief Renders object on the given canvas
     * @param[in,out] canvas Canvas where to render
     */
    virtual void render(Canvas& canvas) = 0;
    virtual ~IRenderable() = default;
};

} // namespace my

#endif // RENDERABLE_H
