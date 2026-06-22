#include <SGE/physics/forceComponents/CFakeStiffSpring3.h>

sge::CFakeStiffSpring3::CFakeStiffSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real damping)
{
    this->anchorPosition = position;
    this->springConstant = springConstant;
    this->damping = damping;
}