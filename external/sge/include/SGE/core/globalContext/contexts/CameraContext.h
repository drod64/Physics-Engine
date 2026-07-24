#ifndef SGE_CAMERA_CONTEXT_H
#define SGE_CAMERA_CONTEXT_H
#include <SGE/core/ecs/Entity.h>

namespace sge {
struct CameraContext {
    Entity activeCamera = sge::Entity::INVALID;
}; // struct CameraContext
} // namespace sge

#endif // SGE_CAMERA_CONTEXT_H