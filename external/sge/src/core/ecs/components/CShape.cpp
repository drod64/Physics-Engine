#include <SGE/core/ecs/components/CShape.h>

namespace sge
{

    CShape::CShape(float radius, int points, sm::real outlineThickness)
    {
        this->radius = radius;
        this->points = radius;
        this->outlineThickness = outlineThickness;
    }

} // namespace sge