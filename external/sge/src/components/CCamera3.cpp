#include <SGE/components/CCamera3.h>

sge::CCamera3::CCamera3(bool isActive, int projectionType, sm::real fov, const sm::Vec3 &up)
{
    this->isActive = isActive;
    this->projection = projectionType;
    this->fov = fov;
    this->up = up;
}