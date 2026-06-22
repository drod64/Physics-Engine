#ifndef SGE_DRAG_SYSTEM3_H
#define SGE_DRAG_SYSTEM3_H
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SM/Vec3.h>
#include <SGE/systems/SystemDescriptor.h>

// Component reads
#include <SGE/physics/forceComponents/CDrag3.h>

// Component writes
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace sge {
    namespace DragSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_DRAG_SYSTEM3_H