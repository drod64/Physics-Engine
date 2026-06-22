#include <SGE/physics/forceComponents/CAnchorSpring3.h>

sge::CAnchorSpring3::CAnchorSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength)
{
    this->anchorPosition = position;
    this->springConstant = springConstant;
    this->restLength = restLength;
}