#include <SGE/physics/forceComponents/CBungee3.h>

sge::CBungee3::CBungee3(Entity targetEntity, sm::real springConstant, sm::real restLength)
{
    this->targetEntity = targetEntity;
    this->springConstant = springConstant;
    this->restLength = restLength;
}