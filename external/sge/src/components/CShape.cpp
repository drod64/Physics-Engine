#include <SGE/Components/CShape.h>

namespace sge
{

    CShape::CShape() :
    CShape(0, 0, 0)
    {}

    CShape::CShape(float radius, int points, sm::real outlineThickness)
    {
        this->radius = radius;
        this->points = radius;
        this->outlineThickness = outlineThickness;
    }

} // namespace sge