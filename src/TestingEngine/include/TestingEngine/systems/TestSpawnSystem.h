#ifndef TEST_SPAWN_SYSTEM_H
#define TEST_SPAWN_SYSTEM_H
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <TestingEngine/ScenePlaySpawn.h>
#include <SGE/systems/SystemDescriptor.h>
#include <TestingEngine/ScenePlayActions.h>

// Reads.
#include <SGE/managers/registryResources/PlayerInputResource.h>

namespace TestSpawnSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt);

    sge::SystemDescriptor getSystemDescriptor();
}

#endif // TEST_SPAWN_SYSTEM_H