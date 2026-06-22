#include <SGE/physics/forceComponents/CBungee3.h>

sge::CBungee3::CBungee3(Entity other, sm::real springConstant, sm::real restLength)
{
    this->other = other;
    this->springConstant = springConstant;
    this->restLength = restLength;
}