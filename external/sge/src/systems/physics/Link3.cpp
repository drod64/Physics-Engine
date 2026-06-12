#include <SGE/systems/physics/Link3.h>

sm::real sge::Link3::currentLength() const
{
    return (this->entities[0]->getComponent<sge::CTransform3>().position -
            this->entities[1]->getComponent<sge::CTransform3>().position).magnitude();
}