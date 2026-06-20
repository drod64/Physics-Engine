#ifndef SGE_ANCHOR_BUNGEE3_H
#define SGE_ANCHOR_BUNGEE3_H
#include <algorithm>
#include <SM/Precision.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Read/Write components
#include <SGE/components/CAnchorBungee3.h>
#include <SGE/components/CRigidBody3.h>
#include <SGE/components/CTransform3.h>

namespace sge {
    namespace AnchorBungeeSystem3 {
        void update(Registry &registry, CommandBuffer &, sm::real dt);

        SystemDescriptor getSystemDescriptor();
    }
}

#endif // SGE_ANCHOR_BUNGEE3_H