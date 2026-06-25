#include <SGE/core/ecs/components/CTransform3.h>

sge::CTransform3::CTransform3()
:
position(0,0,0),
rotation(0,0,0)
{}

sm::Vec3 sge::CTransform3::getForward() const
{
    sm::real degToRad = std::numbers::pi_v<float> / 180.f;

    sm::real radX = this->rotation.x * degToRad;
    sm::real radY = this->rotation.y * degToRad;

    sm::real x = std::cos(radX) *std::sin(radY);
    sm::real y = -std::sin(radX);
    sm::real z = std::cos(radX) * std::cos(radY);

    sm::Vec3 forward(x, y, z);
    forward.normalize();
    
    return forward;
}