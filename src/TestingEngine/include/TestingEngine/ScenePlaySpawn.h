#ifndef SCENE_PLAY_SPAWN
#define SCENE_PLAY_SPAWN
#include <string>
#include <SM/Vec3.h>
#include <SGE/components/CTransform3.h>
#include <SGE/components/CRigidBody3.h>
#include <SGE/components/CLifespan.h>
#include <SGE/core/CommandBuffer.h>
#include <TestingEngine/ActionScenePlay.h>
#include <TestingEngine/ProjectileType.h>

class ScenePlaySpawn {
public:
    static void spawnProjectile(sge::CommandBuffer& cmdBuffer, ProjectileType type);

    static void spawnFirework(sge::CommandBuffer &cmdBuffer, const std::string &tag);

    static void spawnSpringConnection(sge::CommandBuffer &cmdBuffer);

    static void spawnAnchorSpring(sge::CommandBuffer &cmdBuffer, const sm::Vec3 &position);

    static void spawnBungeeSpring(sge::CommandBuffer &cmdBuffer);

    static void spawnBuoyancySpring(sge::CommandBuffer &cmdBuffer);

    static void spawnFakeSpring(sge::CommandBuffer &cmdBuffer);
};

#endif // SCENE_PLAY_SPAWN