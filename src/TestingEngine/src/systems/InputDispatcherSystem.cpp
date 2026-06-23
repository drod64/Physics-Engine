#include <TestingEngine/systems/InputDispatcherSystem.h>

void InputDispatcherSystem::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real)
{
    // Get player input resource.
    auto &playerInput = registry.getOrCreateResource<sge::PlayerInputResource>();

    // Get controller component view.
    auto cView = registry.viewAll<sge::CPlayerController3>();

    for (const sge::Entity &e : cView)
    {
        auto &controller = cView.get<sge::CPlayerController3>(e);

        // Clear actions from last frame.
        controller.clearActions();
        // Reset movement axes to zero
        controller.resetMovementAxes();

        if(playerInput.isActionHeld(ScenePlayAction::MoveForward)) controller.movementAxisZ += 1;
        if(playerInput.isActionHeld(ScenePlayAction::MoveBackward)) controller.movementAxisZ -= 1;
        
        if(playerInput.isActionHeld(ScenePlayAction::MoveUp)) controller.movementAxisY += 1;
        if(playerInput.isActionHeld(ScenePlayAction::MoveDown)) controller.movementAxisY -= 1;
        
        if(playerInput.isActionHeld(ScenePlayAction::MoveRight)) controller.movementAxisX += 1;
        if(playerInput.isActionHeld(ScenePlayAction::MoveLeft)) controller.movementAxisX -= 1;


        // Loop through max possible amount of actions.
        for (uint32_t i = 0; i < 512; ++i)
        {
            if (playerInput.consumeActionPressed(i))
            {
                controller.addActionEvent(sge::ActionEvent(i, sge::ActionType::Pressed));
            }

            if (playerInput.consumeActionReleased(i))
            {
                controller.addActionEvent(sge::ActionEvent(i, sge::ActionType::Released));
            }
        }
    }
}

sge::SystemDescriptor InputDispatcherSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PreUpdate;

    // System functor.
    desc.functionPtr = &InputDispatcherSystem::update;
    
    // No system component reads.

    // System component writes.
    desc.componentWrites.set(sge::ComponentIDCounter::get<sge::CPlayerController3>());

    // System resource reads
    desc.resourceReads.set(sge::ResourceIDCounter::get<sge::PlayerInputResource>());

    // No system resource writes.
    
    // System name.
    desc.name = "PlayerActionSystem";

    return desc;
}