#include <SGE/systems/RenderingSystem3.h>

void sge::RenderingSystem3::update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt)
{
    // Begin looking for camera data.
    auto cameraView = registry.viewAll<sge::CTransform3, sge::CCamera3>();

    sge::CTransform3 t3;
    sge::CCamera3 c3(true, CAMERA_PERSPECTIVE);
    bool cameraFound = false;
    
    for (Entity e : cameraView)
    {
        const auto &camera = cameraView.get<sge::CCamera3>(e);

        if (camera.isActive)
        {
            t3 = cameraView.get<sge::CTransform3>(e);
            c3 = camera;
            cameraFound = true;
            break;
        }
    }

    // Fallback: If no camera exists in this scene, draw basic text or skip 3D
    if (!cameraFound)
    {
        DrawText("NO ACTIVE CAMERA FOUND", 10, 10, 20, RED);
        EndDrawing();
        return;
    }

    // Set up raylib Camera3D.
    Camera3D rayCam;
    rayCam.position = Vector3{t3.position.x, t3.position.y, t3.position.z};
    sm::Vec3 forward = t3.getForward();
    rayCam.target = Vector3{t3.position.x + forward.x,
                            t3.position.y + forward.y,
                            t3.position.z + forward.z};
    rayCam.up = Vector3{c3.up.x, c3.up.y, c3.up.z};
    rayCam.fovy = c3.fov;
    rayCam.projection = c3.projection;
    // End of camera setup. Begin drawing entities.

    BeginMode3D(rayCam);

    // Component pool of sge::CTransform3
    const auto &t3Components = registry.getComponentPool<sge::CTransform3>()->getDenseComponents();

    for (const auto &t3 : t3Components)
    {
        DrawCube({t3.position.x, t3.position.y, t3.position.z}, 2, 2, 2, RED);
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
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CCamera3>());

    // No system component writes.

    // No system component accumulations.

    // Np system resource reads.

    // No system resource writes.

    // No system resource accumulations.

    // System name.
    desc.name = "RenderingSystem3";

    return desc;
}