#include <SGE/components/CTransform.h>

namespace sge 
{
    CTransform::CTransform() :
    position{0,0}, 
    velocity{0,0},
    angle(0.f)
    {}

    CTransform::CTransform(const CTransform &otherTransform) :
    position(otherTransform.position),
    velocity(otherTransform.velocity),
    angle(otherTransform.angle)
    {}

    CTransform::CTransform(const sm::Vec2 &position, const sm::Vec2 &velocity, float angle) :
    position(position),
    velocity(velocity),
    angle(angle)
    {}

    CTransform::~CTransform()
    {}

    CTransform& CTransform::operator = (const CTransform &otherTransform)
    {
        if (this != &otherTransform)
        {
            this->position = otherTransform.position;
            this->velocity = otherTransform.velocity;
            this->angle = otherTransform.angle;
        }
        return *this;
    }
}