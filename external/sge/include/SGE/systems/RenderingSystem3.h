#ifndef SGE_RENDERING_SYSTEM
#define SGE_RENDERING_SYSTEM
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <SM/MathUtil.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Reads.
#include <SGE/core/ecs/components/CCamera3.h>
#include <SGE/core/ecs/components/CTransform3.h>

namespace sge {
    namespace RenderingSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

        SystemDescriptor getSystemDescription();
    };
} // namespace sge

#endif // SGE_RENDERING_SYSTEM