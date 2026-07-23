#include <TestingEngine/ScenePlaySpawn.h>

void ScenePlaySpawn::spawnProjectile(sge::CommandBuffer &cmdBuffer, ProjectileType type)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    
    sge::CRigidBody3 r3(0, false);
    sge::CTransform3 t3;

    switch (type)
    {
        case ProjectileType::PISTOL:
            r3.setMass(2.0f); // 2.0kg
            r3.linearVelocity = sm::Vec3(0.0f, 0.0f, 35.0f); // 35m/s
            break;
        case ProjectileType::ARTILLERY:
            r3.setMass(200.0f); // 200.0kg
            r3.linearVelocity = sm::Vec3(0.0f, 30.0f, 40.0f); // 50m/s
            break;
        case ProjectileType::FIREBALL:
            r3.setMass(1.0f); // 1.0kg - mostly blast damage
            r3.linearVelocity = sm::Vec3(0.0f, 0.0f, 10.0f); // 5m/s
            break;
        case ProjectileType::LASER:
        default:
        r3.setMass(0.1f); // 0.1kg - almost no weight
        r3.linearVelocity = sm::Vec3(0.0f, 0.0f, 100.0f); // 100m/s
        break;
    }

    t3.position = sm::Vec3(0, 1.5, 0);
    t3.prevPosition = t3.position;

    cmdBuffer.addComponentDeferred(e, r3);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, sge::CLifespan(10));
    cmdBuffer.addComponentDeferred(e, sge::CGravity3({0, -9.81, 0}));
}

void ScenePlaySpawn::spawnFirework(sge::CommandBuffer &cmdBuffer, const std::string &tag)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CRigidBody3 r3(0.1, false);
    sge::CTransform3 t3;
    sge::CLifespan ls(100);

    t3.position = {0,0,0};
    t3.prevPosition = t3.position;
    r3.linearVelocity = {0,35,0};

    cmdBuffer.addComponentDeferred(e, r3);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, ls);
}

void ScenePlaySpawn::spawnSpringConnection(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e1 = cmdBuffer.createEntityDeferred();
    sge::CLifespan lse1(10);
    sge::CTransform3 t3e1;
    sge::CRigidBody3 r3_e1(10, false);
    r3_e1.linearVelocity = {0, 20, 25};
    sm::real w_1 = 2.0f, h_1 = 2.0f, d_1 = 2.0f;
    // Raw Inertia formulas for a solid box
    sm::real Ix_1 = (1.0f / 12.0f) * r3_e1.getMass() * (h_1 * h_1 + d_1 * d_1);
    sm::real Iy_1 = (1.0f / 12.0f) * r3_e1.getMass() * (w_1 * w_1 + d_1 * d_1);
    sm::real Iz_1 = (1.0f / 12.0f) * r3_e1.getMass() * (w_1 * w_1 + h_1 * h_1);
    Ix_1 *= 3;
    Iy_1 *= 3;
    Iz_1 *= 3;
    r3_e1.inverseInertiaTensor = { 1.0f / Ix_1, 1.0f / Iy_1, 1.0f / Iz_1 };

    sge::Entity e2 = cmdBuffer.createEntityDeferred();
    sge::CLifespan lse2(10);
    sge::CTransform3 t3e2;
    t3e2.position = {5, 0 ,0};
    t3e2.prevPosition = t3e2.position;
    sge::CRigidBody3 r3_e2(10, false);
    r3_e2.linearVelocity = {0, 0, 0};
    sm::real w_2 = 2.0f, h_2 = 2.0f, d_2 = 2.0f;
    // Raw Inertia formulas for a solid box
    sm::real Ix_2 = (1.0f / 12.0f) * r3_e2.getMass() * (h_2 * h_2 + d_2 * d_2);
    sm::real Iy_2 = (1.0f / 12.0f) * r3_e2.getMass() * (w_2 * w_2 + d_2 * d_2);
    sm::real Iz_2 = (1.0f / 12.0f) * r3_e2.getMass() * (w_2 * w_2 + h_2 * h_2);
    Ix_2 *= 3;
    Iy_2 *= 3;
    Iz_2 *= 3;
    r3_e2.inverseInertiaTensor = { 1.0f / Ix_2, 1.0f / Iy_2, 1.0f / Iz_2 };


    cmdBuffer.addComponentDeferred(e1, lse1);
    cmdBuffer.addComponentDeferred(e1, t3e1);
    cmdBuffer.addComponentDeferred(e1, r3_e1);
    
    cmdBuffer.addComponentDeferred(e2, lse2);
    cmdBuffer.addComponentDeferred(e2, t3e2);
    cmdBuffer.addComponentDeferred(e2, r3_e2);

    cmdBuffer.pushCustomCommand(sge::SpringConstraintCommand {
        .entityA = e1,
        .entityB = e2,
        .springConstant = 65,
        .restLength = 10,
        .localAttachPointA = {-1,-1,-1},
        .localAttachPointB = {1,-1,-1}
    });
}

void ScenePlaySpawn::spawnAnchorSpring(sge::CommandBuffer &cmdBuffer, const sm::Vec3 &position)
{
    // Create entity to be anchored.
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(10);
    sge::CTransform3 t3;
    t3.position = {0, -90, 0};
    t3.prevPosition = t3.position;
    sge::CRigidBody3 r3(5, false);
    sm::real w = 2.0f, h = 2.0f, d = 2.0f;
    // Raw Inertia formulas for a solid box
    sm::real Ix = (1.0f / 12.0f) * r3.getMass() * (h * h + d * d);
    sm::real Iy = (1.0f / 12.0f) * r3.getMass() * (w * w + d * d);
    sm::real Iz = (1.0f / 12.0f) * r3.getMass() * (w * w + h * h);

    Ix *= 3;
    Iy *= 3;
    Iz *= 3;

    r3.inverseInertiaTensor = { 1.0f / Ix, 1.0f / Iy, 1.0f / Iz };

    sge::CAnchorSpring3 springComponent(position, 70, 25);
    springComponent.localAttachPoint = {1,1,1};

    cmdBuffer.addComponentDeferred(e, ls);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, r3);
    cmdBuffer.addComponentDeferred(e, sge::CGravity3({0, -9.81, 0}));
    cmdBuffer.addComponentDeferred(e, sge::CDrag3(0.9, 0.5, 0.9, 0.5));
    cmdBuffer.addComponentDeferred(e, springComponent);
}

void ScenePlaySpawn::spawnAnchorBungee(sge::CommandBuffer &cmdBuffer, const sm::Vec3 &position)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(10);
    sge::CTransform3 t3;
    t3.position = {0, 0, 0};
    t3.prevPosition = t3.position;
    sge::CRigidBody3 r3(40, false);
    r3.linearVelocity.y = 30;
    r3.linearVelocity.z = 30;
    r3.angularVelocity.z = 5;
    sm::real w = 2.0f, h = 2.0f, d = 2.0f;
    // Raw Inertia formulas for a solid box
    sm::real Ix = (1.0f / 12.0f) * r3.getMass() * (h * h + d * d);
    sm::real Iy = (1.0f / 12.0f) * r3.getMass() * (w * w + d * d);
    sm::real Iz = (1.0f / 12.0f) * r3.getMass() * (w * w + h * h);

    Ix *= 3;
    Iy *= 3;
    Iz *= 3;

    r3.inverseInertiaTensor = { 1.0f / Ix, 1.0f / Iy, 1.0f / Iz };

    sge::CAnchorBungee3 bungeeComponent(position, 70, 25);
    bungeeComponent.localAttachPoint = {0, 1.5, 0};

    cmdBuffer.addComponentDeferred(e, bungeeComponent);
    cmdBuffer.addComponentDeferred(e, sge::CGravity3({0, -9.81, 0}));
    cmdBuffer.addComponentDeferred(e, sge::CDrag3(0.3, 0.05, 0.4, 0.05));
    cmdBuffer.addComponentDeferred(e, ls);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, r3);
}

void ScenePlaySpawn::spawnBungeeSpring(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e1 = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls_e1(10);
    sge::CTransform3 t3_e1;
    t3_e1.position = {0, 25, 0};
    t3_e1.prevPosition = t3_e1.position;
    sge::CRigidBody3 r3_e1(10, false);
    r3_e1.linearVelocity = {0, 10, 10};
    sm::real w_1 = 2.0f, h_1 = 2.0f, d_1 = 2.0f;
    // Raw Inertia formulas for a solid box
    sm::real Ix_1 = (1.0f / 12.0f) * r3_e1.getMass() * (h_1 * h_1 + d_1 * d_1);
    sm::real Iy_1 = (1.0f / 12.0f) * r3_e1.getMass() * (w_1 * w_1 + d_1 * d_1);
    sm::real Iz_1 = (1.0f / 12.0f) * r3_e1.getMass() * (w_1 * w_1 + h_1 * h_1);
    Ix_1 *= 3;
    Iy_1 *= 3;
    Iz_1 *= 3;
    r3_e1.inverseInertiaTensor = { 1.0f / Ix_1, 1.0f / Iy_1, 1.0f / Iz_1 };

    sge::Entity e2 = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls_e2(10);
    sge::CTransform3 t3_e2;
    t3_e2.position = {0, 30, 0};
    t3_e2.prevPosition = t3_e2.position;
    sge::CRigidBody3 r3_e2(10, false);
    r3_e2.addForce({0, 0, 100});
    sm::real w_2 = 2.0f, h_2 = 2.0f, d_2 = 2.0f;
    // Raw Inertia formulas for a solid box
    sm::real Ix_2 = (1.0f / 12.0f) * r3_e2.getMass() * (h_2 * h_2 + d_2 * d_2);
    sm::real Iy_2 = (1.0f / 12.0f) * r3_e2.getMass() * (w_2 * w_2 + d_2 * d_2);
    sm::real Iz_2 = (1.0f / 12.0f) * r3_e2.getMass() * (w_2 * w_2 + h_2 * h_2);
    Ix_2 *= 3;
    Iy_2 *= 3;
    Iz_2 *= 3;
    r3_e2.inverseInertiaTensor = { 1.0f / Ix_2, 1.0f / Iy_2, 1.0f / Iz_2 };

    cmdBuffer.addComponentDeferred(e1, ls_e1);
    cmdBuffer.addComponentDeferred(e1, t3_e1);
    cmdBuffer.addComponentDeferred(e1, r3_e1);
    cmdBuffer.addComponentDeferred(e1, sge::CGravity3({0, -9.81, 0}));

    cmdBuffer.addComponentDeferred(e2, ls_e2);
    cmdBuffer.addComponentDeferred(e2, t3_e2);
    cmdBuffer.addComponentDeferred(e2, r3_e2);
    cmdBuffer.addComponentDeferred(e2, sge::CGravity3({0, -9.81, 0}));

    cmdBuffer.pushCustomCommand(sge::BungeeConstraintCommand {
        .entityA = e1,
        .entityB = e2,
        .springConstant = 45,
        .restLength = 15,
        .localAttachPointA = {-1,-1,-1},
        .localAttachPointB = {1,-1,-1}
    });
}

void ScenePlaySpawn::spawnBuoyancySpring(sge::CommandBuffer &cmdBuffer)
{
    sge::Entity e = cmdBuffer.createEntityDeferred();
    sge::CLifespan ls(10);
    sge::CTransform3 t3;
    t3.position = {0, 4, 0};
    t3.prevPosition = t3.position;
    sge::CRigidBody3 r3(10, false);

    cmdBuffer.addComponentDeferred(e, ls);
    cmdBuffer.addComponentDeferred(e, t3);
    cmdBuffer.addComponentDeferred(e, r3);
    cmdBuffer.addComponentDeferred(e, sge::CDrag3(0.9, 0.5, 0.9, 0.5));
    cmdBuffer.addComponentDeferred(e, sge::CGravity3({0, -9.81, 0}));
    cmdBuffer.addComponentDeferred(e, sge::CBuoyancy3(-50, 100, -10, 100));
}