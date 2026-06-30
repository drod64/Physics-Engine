#ifndef SCENE_PLAY_SPAWN
#define SCENE_PLAY_SPAWN
#include <string>
#include <SM/Vec3.h>
#include <SGE/managers/CommandBuffer.h>
#include <TestingEngine/ProjectileType.h>
#include <SGE/physics/constraints/commands/SpringConstraintCommand.h>

// Components
#include <SGE/physics/forceComponents/CRigidBody3.h>
#include <SGE/physics/forceComponents/CGravity3.h>
#include <SGE/physics/forceComponents/CAnchorSpring3.h>
#include <SGE/physics/forceComponents/CSpring3.h>
#include <SGE/physics/forceComponents/CAnchorBungee3.h>
#include <SGE/physics/forceComponents/CBungee3.h>
#include <SGE/physics/forceComponents/CDrag3.h>
#include <SGE/physics/forceComponents/CBuoyancy3.h>
#include <SGE/core/ecs/components/CLifespan.h>
#include <SGE/core/ecs/components/CTransform3.h>

class ScenePlaySpawn {
public:
    static void spawnProjectile(sge::CommandBuffer& cmdBuffer, ProjectileType type);

    static void spawnFirework(sge::CommandBuffer &cmdBuffer, const std::string &tag);

    static void spawnSpringConnection(sge::CommandBuffer &cmdBuffer);

    static void spawnAnchorSpring(sge::CommandBuffer &cmdBuffer, const sm::Vec3 &position);

    static void spawnAnchorBungee(sge::CommandBuffer &cmdBuffer, const sm::Vec3 &position);

    static void spawnBungeeSpring(sge::CommandBuffer &cmdBuffer);

    static void spawnBuoyancySpring(sge::CommandBuffer &cmdBuffer);
};

#endif // SCENE_PLAY_SPAWN