#include <SGE/physics/forceComponents/CSpring3.h>

sge::CSpring3::CSpring3(Entity targetEntity, sm::real springConstant, sm::real restLength)
{
    this->targetEntity = targetEntity;
    this->springConstant = springConstant;
    this->restLength = restLength;
}