#ifndef SGE_PLAYER_MOVEMENT_SYSTEM_H
#define SGE_PLAYER_MOVEMENT_SYSTEM_H
#include <SGE/core/registryResources/PlayerInputResource.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SM/Vec3.h>

namespace sge {
    namespace PlayerMovementSystem {
        void update(Registry &registry, CommandBuffer &, sm::real dt);
    }
}

#endif // SGE_MOVEMENT_SYSTEM_H