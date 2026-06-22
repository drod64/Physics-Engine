#ifndef SGE_FAKE_STIFF_SPRING_SYSTEM3_H
#define SGE_FAKE_STIFF_SPRING_SYSTEM3_H
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SM/Vec3.h>
#include <SGE/systems/SystemDescriptor.h>

// Component reads
#include <SGE/core/ecs/components/CTransform3.h>
#include <SGE/physics/forceComponents/CFakeStiffSpring3.h>

// Component writes
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace sge {
    namespace FakeStiffSpringSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
} // namespace sge

#endif // SGE_FAKE_SPRING3_H