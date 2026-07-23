#ifndef SGE_SPRING_SYSTEM3_H
#define SGE_SPRING_SYSTEM3_H
#include <SM/Vec3.h>
#include <SGE/physics/RigidBodyUtils.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Component reads
#include <SGE/core/ecs/components/CTransform3.h>
#include <SGE/physics/constraints/types/SpringConstraint.h>

// Component writes
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace sge {
    namespace SpringSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);
        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_SPRING_FORCE3_H