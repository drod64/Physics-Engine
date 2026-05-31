#ifndef SGE_CSHAPE_COMPONENT_H
#define SGE_CSHAPE_COMPONENT_H
#include <SGE/components/Component.h>

namespace sge
{

class CShape : public Component {
public:
    

    CShape();
    CShape(float radius, int points, float outlineThickness);
};

} // namespace sge

#endif // SGE_CSHAPE_COMPONENT_H