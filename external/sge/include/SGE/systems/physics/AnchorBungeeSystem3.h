#ifndef SGE_ANCHOR_BUNGEE_SYSTEM3_H
#define SGE_ANCHOR_BUNGEE_SYSTEM3_H
#include <algorithm>
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Read/Write components
#include <SGE/physics/forceComponents/CAnchorBungee3.h>
#include <SGE/physics/forceComponents/CRigidBody3.h>
#include <SGE/core/ecs/components/CTransform3.h>

namespace sge {
    namespace AnchorBungeeSystem3 {
        void update(Registry &registry, CommandBuffer &, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_ANCHOR_BUNGEE_SYSTEM3_H