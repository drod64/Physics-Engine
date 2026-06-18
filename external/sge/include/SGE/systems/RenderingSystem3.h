#ifndef SGE_RENDERING_SYSTEM
#define SGE_RENDERING_SYSTEM
#include <raylib.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SM/Precision.h>
#include <SGE/components/CTransform3.h>
#include <SGE/components/CCamera3.h>

namespace sge {
    namespace RenderingSystem3 {
        void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);
    };
} // namespace sge

#endif // SGE_RENDERING_SYSTEM