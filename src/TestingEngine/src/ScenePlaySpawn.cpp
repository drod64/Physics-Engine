#include <TestingEngine/ScenePlaySpawn.h>

void ScenePlaySpawn::spawnProjectile(sge::CommandBuffer &cmdBuffer, ProjectileType type)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    
    sge::CRigidBody3 r3;
    sge::CTransform3 t3;

    switch (type)
    {
        case ProjectileType::PISTOL:
            r3.setMass(2.0f); // 2.0kg
            r3.velocity = sm::Vec3(0.0f, 0.0f, 35.0f); // 35m/s
            r3.damping = 0.99;
            break;
        case ProjectileType::ARTILLERY:
            r3.setMass(200.0f); // 200.0kg
            r3.velocity = sm::Vec3(0.0f, 30.0f, 40.0f); // 50m/s
            r3.damping = 0.99;
            break;
        case ProjectileType::FIREBALL:
            r3.setMass(1.0f); // 1.0kg - mostly blast damage
            r3.velocity = sm::Vec3(0.0f, 0.0f, 10.0f); // 5m/s
            r3.damping = 0.99;
            break;
        case ProjectileType::LASER:
        default:
        r3.setMass(0.1f); // 0.1kg - almost no weight
        r3.velocity = sm::Vec3(0.0f, 0.0f, 100.0f); // 100m/s
        r3.damping = 0.99;
        break;
    }

    t3.position = sm::Vec3(0, 1.5, 0);

    cmdBuffer.addComponentDeferred(e, r3);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, sge::CLifespan(600));
    cmdBuffer.addComponentDeferred(e, sge::CGravity3({0, -9.81, 0}));
}

void ScenePlaySpawn::spawnFirework(sge::CommandBuffer &cmdBuffer, const std::string &tag)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CRigidBody3 r3;
    sge::CTransform3 t3;
    sge::CLifespan ls(100);

    t3.position = {0,0,0};
    r3.setMass(0.1);
    r3.velocity = {0,35,0};
    r3.damping = 0.3;

    cmdBuffer.addComponentDeferred(e, r3);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, ls);
}

void ScenePlaySpawn::spawnSpringConnection(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e1 = cmdBuffer.createEntityDeferred();
    sge::CLifespan lse1(500);
    sge::CTransform3 t3e1;
    sge::CRigidBody3 r3e1;
    r3e1.setMass(10);
    r3e1.velocity = {0, 20, 25};
    cmdBuffer.addComponentDeferred(e1, lse1);
    cmdBuffer.addComponentDeferred(e1, t3e1);
    cmdBuffer.addComponentDeferred(e1, r3e1);
    
    sge::Entity e2 = cmdBuffer.createEntityDeferred();
    sge::CLifespan lse2(500);
    sge::CTransform3 t3e2;
    t3e2.position = {5, 0 ,0};
    sge::CRigidBody3 r3e2;
    r3e2.setMass(10);
    r3e2.velocity = {0, 0, 0};
    cmdBuffer.addComponentDeferred(e2, lse2);
    cmdBuffer.addComponentDeferred(e2, t3e2);
    cmdBuffer.addComponentDeferred(e2, r3e2);
}

void ScenePlaySpawn::spawnAnchorSpring(sge::CommandBuffer &cmdBuffer, const sm::Vec3 &position)
{
    // Create entity to be anchored.
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(600);
    sge::CTransform3 t3;
    t3.position = {0, -90, 0};
    sge::CRigidBody3 r3;
    r3.setMass(5);

    cmdBuffer.addComponentDeferred(e, ls);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, r3);
}

void ScenePlaySpawn::spawnBungeeSpring(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e1 = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(600);
    sge::CTransform3 t3;
    t3.position = {0, 5, 0};
    sge::CRigidBody3 r3;
    r3.addForce({0, 0, 100});
    r3.setMass(10);

    cmdBuffer.addComponentDeferred(e1, ls);
    cmdBuffer.addComponentDeferred(e1, t3);
    cmdBuffer.addComponentDeferred(e1, r3);
    cmdBuffer.addComponentDeferred(e1, sge::CAnchorBungee3({0, 10, 0}, 40, 15));
    cmdBuffer.addComponentDeferred(e1, sge::CGravity3({0, -9.81, 0}));
}

void ScenePlaySpawn::spawnBuoyancySpring(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(600);
    sge::CTransform3 t3;
    t3.position = {0, 4, 0};
    sge::CRigidBody3 r3;
    r3.setMass(10);

    cmdBuffer.addComponentDeferred(e, ls);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, r3);
}

void ScenePlaySpawn::spawnFakeSpring(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(600);
    sge::CTransform3 t3;
    t3.position = {0, 4, 0};
    sge::CRigidBody3 r3;
    r3.setMass(10);

    cmdBuffer.addComponentDeferred(e, ls);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, r3);
}