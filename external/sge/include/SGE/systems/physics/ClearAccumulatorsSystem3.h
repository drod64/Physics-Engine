#ifndef SGE_CLEAR_ACCUMULATORS_SYSTEM3_H
#define SGE_CLEAR_ACCUMULATORS_SYSTEM3_H
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SM/Vec3.h>
#include <SGE/systems/SystemDescriptor.h>

// Component writes
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace sge {
    namespace ClearAccumulatorsSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_CLEAR_ACCUMULATORS_SYSTEM3_H