#include <SGE/components/CSpring3.h>

sge::CSpring3::CSpring3(Entity other, sm::real springConstant, sm::real restLength)
{
    this->other = other;
    this->springConstant = springConstant;
    this->restLength = restLength;
}