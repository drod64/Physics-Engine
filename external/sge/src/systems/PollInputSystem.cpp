#include <SGE/systems/PollInputSystem.h>
#include <iostream>

void sge::PollInputSystem::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    const auto &inputMapper = registry.getOrCreateResource<sge::InputMappingResource>();
    auto &playerInput = registry.getOrCreateResource<sge::PlayerInputResource>();

    // Prepare snapshot for polling.
    playerInput.prepareForPolling();

    // Update current key input snapshot.
    for (const sge::ButtonBind &keyBind : inputMapper.getKeyBinds())
    {
        playerInput.setAction(keyBind.actionID, IsKeyDown(keyBind.button));
    }
    
    // Update current mouse input snapshot.
    for (const sge::ButtonBind &mouseBind : inputMapper.getMouseBinds())
    {
        playerInput.setAction(mouseBind.actionID, IsKeyDown(mouseBind.button));
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