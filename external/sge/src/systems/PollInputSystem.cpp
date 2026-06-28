#include <SGE/systems/PollInputSystem.h>

void sge::PollInputSystem::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    const auto &inputMapper = registry.getContext<sge::InputMappingContext>();
    auto &playerInput = registry.getContext<sge::PlayerInputContext>();

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

    // System context reads.
    desc.contextReads.set(sge::GlobalContextIDCounter::get<sge::InputMappingContext>());
    
    // No system context accumulation.
    
    // System context writes.
    desc.contextWrites.set(sge::GlobalContextIDCounter::get<sge::PlayerInputContext>());

    // System name.
    desc.name = "PollInputSystem";

    return desc;
}