#ifndef TESTING_ENGINE_CCAMERA_FOLLOW_3_H
#define TESTING_ENGINE_CCAMERA_FOLLOW_3_H
#include <SM/Vec3.h>
#include <SGE/core/ecs/Entity.h>

struct CCameraFollow3 {
    sge::Entity targetEntity = sge::Entity::INVALID;
    sm::Vec3 offset = {0, 0, 0};
    sm::real smoothSpeed = static_cast<sm::real>(10);
}; // struct CCameraFollow3

#endif // TESTING_ENGINE_CCAMERA_FOLLOW_3_H