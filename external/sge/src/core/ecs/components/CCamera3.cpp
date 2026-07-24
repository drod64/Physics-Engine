#include <SGE/core/ecs/components/CCamera3.h>

sge::CCamera3::CCamera3(bool isActive, int projectionType, sm::real fov)
{
    this->isActive = isActive;
    this->projection = projectionType;
    this->fov = fov;
}