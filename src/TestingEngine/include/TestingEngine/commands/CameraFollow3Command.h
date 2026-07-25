#ifndef TESTING_ENGINE_CAMERA_FOLLOW3_COMMAND_H
#define TESTING_ENGINE_CAMERA_FOLLOW3_COMMAND_H
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/commands/CommandContext.h>
#include <TestingEngine/components/CCameraFollow3.h>

struct CameraFollow3Command {
    sge::Entity cameraEntity = sge::Entity::INVALID;
    sge::Entity targetEntity = sge::Entity::INVALID;
    sm::Vec3 offset = {0, 0, 0};
    sm::real smoothSpeed = static_cast<sm::real>(10);

    void execute(sge::CommandContext &context) const
    {
        sge::Entity realCamera = context.getEntity(this->cameraEntity);
        sge::Entity realTarget = context.getEntity(this->targetEntity);

        if (realCamera == sge::Entity::INVALID ||
            realTarget == sge::Entity::INVALID) return;
    
        CCameraFollow3 follow {
            .targetEntity = realTarget,
            .offset = this->offset,
            .smoothSpeed = this->smoothSpeed
        };

        auto &registry = context.getRegistry();

        // Check if component already exists.
        if (registry.hasComponent<CCameraFollow3>(realCamera))
        {
            registry.replaceComponent(realCamera, follow);
        }
        // If not, add it.
        else
        {
            registry.addComponent(realCamera, follow);
        }

    }
}; // struct CameraFollow3Command

#endif // TESTING_ENGINE_CAMERA_FOLLOW3_COMMAND_H