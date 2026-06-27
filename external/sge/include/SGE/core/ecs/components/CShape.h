#ifndef SGE_CSHAPE_COMPONENT_H
#define SGE_CSHAPE_COMPONENT_H
#include <SM/Precision.h>

namespace sge
{
/**
 * Component that stores shape data.
 */
class CShape {
public:
    int radius;
    int points;
    sm::real outlineThickness;

    /**
     * Default Constructor.
     */
    CShape() = default;

    /**
     * Parameterized Constructor.
     * @param radius the radius of the shape
     * @param points the number of points the shape contains
     * @param outlineThickness the outline thickness the shape has
     */
    CShape(float radius, int points, sm::real outlineThickness);
};

} // namespace sge

#endif // SGE_CSHAPE_COMPONENT_H