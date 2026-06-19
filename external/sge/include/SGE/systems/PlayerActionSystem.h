#ifndef SGE_PLAYER_ACTION_SYSTEM_H
#define SGE_PLAYER_ACTION_SYSTEM_H
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SM/Precision.h>
#include <SGE/components/CPlayer.h>
#include <SGE/core/registryResources/PlayerInputResource.h>
#include <SGE/components/CTransform3.h>
#include <SGE/components/CCamera3.h>
#include <raylib.h>

namespace sge {
    namespace PlayerActionSystem {
        void update(Registry &registry, CommandBuffer &cmd, sm::real);
    };
}

#endif // SGE_PLAYER_ACTION_SYSTEM_H