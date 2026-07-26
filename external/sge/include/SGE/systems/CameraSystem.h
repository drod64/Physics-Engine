#ifndef SGE_CAMERA_SYSTEM_H
#define SGE_CAMERA_SYSTEM_H
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>
#include <SGE/physics/Directions3.h>

// Reads
#include <SGE/core/globalContext/contexts/CameraContext.h>
#include <SGE/core/ecs/components/CTransform3.h>

// Write
#include <SGE/core/ecs/components/CCamera3.h>

namespace sge {
    namespace CameraSystem {
        void update(Registry &registry, CommandBuffer &buffer, sm::real dt);

        SystemDescriptor getSystemDescription();

    } // namespace CameraSystem
} // namespace sge

#endif // SGE_CAMERA_SYSTEM_H