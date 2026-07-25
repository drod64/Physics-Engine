#include <SGE/systems/RenderingSystem3.h>

void sge::RenderingSystem3::update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt)
{
    sge::Entity activeCamera = registry.getContext<sge::CameraContext>().activeCamera;

    // Fallback: If no camera exists in this scene, draw basic text or skip 3D
    if (activeCamera == sge::Entity::INVALID ||
        !registry.hasComponent<sge::CTransform3>(activeCamera) ||
        !registry.hasComponent<sge::CCamera3>(activeCamera))
    {
        DrawText("NO ACTIVE CAMERA FOUND", 10, 10, 20, RED);
        return;
    }

    const auto &t3 = registry.getComponent<sge::CTransform3>(activeCamera);
    auto &c3 = registry.getComponent<sge::CCamera3>(activeCamera);

    c3.forward = t3.orientation * sm::Vec3(0, 0, 1);
    c3.up = t3.orientation * sm::Vec3(0, 1, 0);
    // Set up raylib Camera3D.
    Camera3D rayCam = { 0 };
    rayCam.position = Vector3{t3.position.x, t3.position.y, t3.position.z};
    rayCam.target = Vector3{t3.position.x + c3.forward.x,
                            t3.position.y + c3.forward.y,
                            t3.position.z + c3.forward.z};
    rayCam.up = Vector3{c3.up.x, c3.up.y, c3.up.z};
    rayCam.fovy = c3.fov;
    rayCam.projection = c3.projection;
    // End of camera setup. Begin drawing entities.

    BeginMode3D(rayCam);

    // Component pool of sge::CTransform3
    const auto &t3Components = registry.getComponentPool<sge::CTransform3>()->getDenseComponents();

    for (const auto &t3 : t3Components)
    {
        sm::Vec3 renderPos = sm::MathUtil::lerp(t3.prevPosition, t3.position, dt);

        // TODO: Optimize drawing
        Quaternion raylibQuat = { t3.orientation.x, t3.orientation.y, t3.orientation.z, t3.orientation.w };
        Matrix rotationMatrix = QuaternionToMatrix(raylibQuat);

        rlPushMatrix();
        
        rlTranslatef(renderPos.x, renderPos.y, renderPos.z);
        rlMultMatrixf(MatrixToFloat(rotationMatrix));

        DrawCube({0, 0, 0}, 2, 2, 2, RED);
        DrawSphere({0.0f, 1.5f, 0.0f}, 0.3f, BLUE); 
        DrawCubeWires({0,0,0}, 2, 2, 2, BLACK);

        rlPopMatrix();
    }

    EndMode3D();
}

sge::SystemDescriptor sge::RenderingSystem3::getSystemDescription()
{
    SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PostUpdate;

    // System functor.
    desc.functionPtr = &sge::RenderingSystem3::update;

    // System component reads.
    desc.components.reads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    
    // No system component writes.
    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CCamera3>());

    // No system component accumulations.

    // Np system resource reads.

    // No system resource writes.

    // No system resource accumulations.

    // System name.
    desc.name = "RenderingSystem3";

    return desc;
}