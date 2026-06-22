#include <SGE/systems/PollInputSystem.h>
#include <iostream>

void sge::PollInputSystem::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    const auto &inputMapper = registry.getOrCreateResource<sge::InputMappingResource>();
    auto &playerInput = registry.getOrCreateResource<sge::PlayerInputResource>();

    // Save the previous frame's snapshot.
    playerInput.updatePrevious();
    playerInput.reset();

    // Update current snapshot.
    for (const sge::ButtonBind &keyBind : inputMapper.getKeyBinds())
    {
        playerInput.setAction(keyBind.actionID, IsKeyDown(keyBind.button));
    }
    
    for (const sge::ButtonBind &mouseBind : inputMapper.getMouseBinds())
    {
        playerInput.setAction(mouseBind.actionID, IsMouseButtonDown(mouseBind.button));
    }

    Vector2 mouseDelta = GetMouseDelta();
    Vector2 mousePosition = GetMousePosition();

    playerInput.setMouseDelta({mouseDelta.x, mouseDelta.y});
    playerInput.setMousePosition({mousePosition.x, mousePosition.y});
}

sge::SystemDescriptor sge::PollInputSystem::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PreUpdate;

    // System functor.
    desc.functionPtr = &sge::PollInputSystem::update;

    // No system component reads.
    
    // No system component accumulation.

    // No system component writes.

    // System resource reads.
    desc.componentReads.set(sge::ResourceIDCounter::get<sge::InputMappingResource>());

    // No system resource accumulation.

    // System resource writes.
    desc.componentAccumulates.set(sge::ResourceIDCounter::get<sge::PlayerInputResource>());

    // System name.
    desc.name = "PollInputSystem";

    return desc;
}