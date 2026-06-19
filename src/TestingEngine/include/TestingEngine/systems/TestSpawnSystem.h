#ifndef TEST_SPAWN_SYSTEM_H
#define TEST_SPAWN_SYSTEM_H
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SM/Precision.h>
#include <SGE/core/registryResources/PlayerInputResource.h>
#include <TestingEngine/ScenePlaySpawn.h>

namespace TestSpawnSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt);
}

#endif // TEST_SPAWN_SYSTEM_H