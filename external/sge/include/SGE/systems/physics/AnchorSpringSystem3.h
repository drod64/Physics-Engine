#ifndef SGE_ANCHOR_SPRING_SYSTEM3_H
#define SGE_ANCHOR_SPRING_SYSTEM3_H
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SM/Vec3.h>
#include <SGE/systems/SystemDescriptor.h>

// Component Reads
#include <SGE/core/ecs/components/CTransform3.h>
#include <SGE/physics/forceComponents/CAnchorSpring3.h>

// Component Writes
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace sge {
    namespace AnchorSpringSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescriptor();
};
} // namespace sge

#endif // SGE_ANCHOR_SPRING_SYSTEM3_H