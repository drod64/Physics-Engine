#ifndef SGE_INTEGRATION_SYSTEM3
#define SGE_INTEGRATION_SYSTEM3
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SM/Vec3.h>
#include <SGE/systems/SystemDescriptor.h>

// Component writes
#include <SGE/physics/forceComponents/CRigidBody3.h>
#include <SGE/core/ecs/components/CTransform3.h>

namespace sge {
    namespace IntegrationSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_INTEGRATION_SYSTEM3