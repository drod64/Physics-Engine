#include <SGE/physics/forceComponents/CAnchorBungee3.h>

sge::CAnchorBungee3::CAnchorBungee3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength)
{
    this->anchorPosition = position;
    this->springConstant = springConstant;
    this->restLength = restLength;
}