#include <SGE/systems/RenderingSystem3.h>

void sge::RenderingSystem3::update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt)
{
    // 1. Set-up viewport camera.
    sge::Entity activeCamera = registry.getContext<sge::CameraContext>().activeCamera;

    // Fallback: If no camera exists in this scene, draw basic text or skip 3D
    if (activeCamera == sge::Entity::INVALID ||
        !registry.hasComponent<sge::CTransform3>(activeCamera) ||
        !registry.hasComponent<sge::CCamera3>(activeCamera))
    {
        DrawText("NO ACTIVE CAMERA FOUND", 10, 10, 20, RED);
        return;
    }

    const auto &cameraT3 = registry.getComponent<sge::CTransform3>(activeCamera);
    auto &cameraC3 = registry.getComponent<sge::CCamera3>(activeCamera);

    cameraC3.forward = cameraT3.orientation * sm::Vec3(0, 0, 1);
    cameraC3.up = cameraT3.orientation * sm::Vec3(0, 1, 0);
    // Set up raylib Camera3D.
    Camera3D rayCam = { 0 };
    rayCam.position = Vector3{cameraT3.position.x, cameraT3.position.y, cameraT3.position.z};
    rayCam.target = Vector3{cameraT3.position.x + cameraC3.forward.x,
                            cameraT3.position.y + cameraC3.forward.y,
                            cameraT3.position.z + cameraC3.forward.z};
    rayCam.up = Vector3{cameraC3.up.x, cameraC3.up.y, cameraC3.up.z};
    rayCam.fovy = cameraC3.fov;
    rayCam.projection = cameraC3.projection;
    // End of camera setup. Begin drawing entities.

    BeginMode3D(rayCam);

    // 2. Draw entities with transforms.
    auto renderView = registry.viewAll<sge::CTransform3>();
    for (sge::Entity e : renderView)
    {
        if (e == activeCamera) continue;

        auto &renderT3 = renderView.get<sge::CTransform3>(e);
        sm::Vec3 renderPos = sm::MathUtil::lerp(renderT3.prevPosition, renderT3.position, dt);

        // TODO: Optimize drawing
        Quaternion raylibQuat = { renderT3.orientation.x, renderT3.orientation.y, renderT3.orientation.z, renderT3.orientation.w };
        Matrix rotationMatrix = QuaternionToMatrix(raylibQuat);

        // World Axes.
        DrawLine3D({0, 0, 0}, {5, 0, 0}, GREEN);
        DrawLine3D({0, 0, 0}, {0, 5, 0}, RED);
        DrawLine3D({0, 0, 0}, {0, 0, 5}, BLUE);

        rlPushMatrix();
        rlTranslatef(renderPos.x, renderPos.y, renderPos.z);
        rlMultMatrixf(MatrixToFloat(rotationMatrix));

        DrawCube({0, 0, 0}, 2, 2, 2, RED);
        DrawSphere({0, 1.5, 0}, 0.3f, BLUE); 
        DrawCubeWires({0,0,0}, 2, 2, 2, BLACK);
        // Local axes.
        DrawLine3D({0, 0, 0}, {5, 0, 0}, GREEN);
        DrawLine3D({0, 0, 0}, {0, 5, 0}, RED);
        DrawLine3D({0, 0, 0}, {0, 0, 5}, BLUE);

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