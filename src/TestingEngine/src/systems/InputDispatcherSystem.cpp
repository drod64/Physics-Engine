#include <TestingEngine/systems/InputDispatcherSystem.h>

void InputDispatcherSystem::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real)
{
    // Get player input resource.
    auto &playerInput = registry.getContext<sge::PlayerInputContext>();

    // Get controller component view.
    auto cView = registry.viewAll<sge::CPlayerController3>();

    for (const sge::Entity &e : cView)
    {
        auto &controller = cView.get<sge::CPlayerController3>(e);

        // Clear actions from last frame.
        controller.clearActions();
        // Reset movement axes to zero
        controller.resetMovementAxes();

        if(playerInput.isActionHeld(ScenePlayAction::MoveForward)) controller.movementAxisZ += sge::Directions3::getForwardSign();
        if(playerInput.isActionHeld(ScenePlayAction::MoveBackward)) controller.movementAxisZ += sge::Directions3::getBackwardSign();
        
        if(playerInput.isActionHeld(ScenePlayAction::MoveUp)) controller.movementAxisY += sge::Directions3::getUpSign();
        if(playerInput.isActionHeld(ScenePlayAction::MoveDown)) controller.movementAxisY += sge::Directions3::getDownSign();
        
        if(playerInput.isActionHeld(ScenePlayAction::MoveRight)) controller.movementAxisX += sge::Directions3::getRightSign();
        if(playerInput.isActionHeld(ScenePlayAction::MoveLeft)) controller.movementAxisX += sge::Directions3::getLeftSign();


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
    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CPlayerController3>());

    // System context reads.
    desc.contexts.reads.set(sge::GlobalContextIDCounter::get<sge::PlayerInputContext>());

    // No system context writes.
    
    // System name.
    desc.name = "PlayerActionSystem";

    return desc;
}