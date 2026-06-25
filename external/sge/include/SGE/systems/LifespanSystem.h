#ifndef SGE_LIFESPAN_SYSTEM_H
#define SGE_LIFESPAN_SYSTEM_H
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Accumulation
#include <SGE/core/ecs/components/CLifespan.h>

namespace sge {
    namespace LifespanSystem {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescription();
    }
}

#endif // SGE_LIFESPAN_SYSTEM_H