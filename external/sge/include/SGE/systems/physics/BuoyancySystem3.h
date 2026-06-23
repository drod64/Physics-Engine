#ifndef SGE_BUOYANCY_SYSTEM3_H
#define SGE_BUOYANCY_SYSTEM3_H
#include <algorithm>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SM/Vec3.h>
#include <SGE/systems/SystemDescriptor.h>

// Component reads
#include <SGE/core/ecs/components/CTransform3.h>
#include <SGE/physics/forceComponents/CBuoyancy3.h>

// Component writes
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace sge {
    namespace BuoyancySystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_BUOYANCY_SYSTEM3_H