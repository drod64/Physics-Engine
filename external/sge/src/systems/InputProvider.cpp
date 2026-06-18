#include <SGE/systems/InputProvider.h>

sge::InputProvider::InputProvider()
{
    // Movement actions
    this->m_keyToAction[KEY_W] = sge::Action::MoveForward;
    this->m_keyToAction[KEY_S] = sge::Action::MoveBackward;
    this->m_keyToAction[KEY_D] = sge::Action::MoveRight;
    this->m_keyToAction[KEY_A] = sge::Action::MoveLeft;
    this->m_keyToAction[KEY_LEFT_SHIFT] = sge::Action::Sprint;
    this->m_keyToAction[KEY_RIGHT_SHIFT] = sge::Action::Sprint;
    this->m_keyToAction[KEY_LEFT_CONTROL] = sge::Action::Crouch;
    this->m_keyToAction[KEY_RIGHT_CONTROL] = sge::Action::Crouch;

    // Interaction actions
    this->m_keyToAction[KEY_SPACE] = sge::Action::Jump;
    this->m_keyToAction[KEY_E] = sge::Action::Interact;
    this->m_keyToAction[KEY_BACKSPACE] = sge::Action::Cancel;

    // Execute actions
    this->m_mouseToAction[MOUSE_BUTTON_LEFT] = sge::Action::PrimaryAction;
    this->m_mouseToAction[MOUSE_BUTTON_RIGHT] = sge::Action::SecondaryAction;
    this->m_keyToAction[KEY_R] = sge::Action::Reload;

    // Global state actions
    this->m_keyToAction[KEY_P] = sge::Action::Pause;
    this->m_keyToAction[KEY_ONE] = sge::Action::MenuToggle;
}

void sge::InputProvider::updateInputSnapshot(sge::InputActionSnapshot &input)
{
    // Check for keyboard input
    for (const auto& [key, action] : this->m_keyToAction)
    {
        input.setAction(action, IsKeyDown(key));
    }

    // Check for mouse input
    for (const auto& [mouse, action] : this->m_mouseToAction)
    {
        input.setAction(action, IsMouseButtonDown(mouse));
    }

    // Update mouse delta and position
    input.mouseDelta = sm::Vec2(GetMouseDelta().x, GetMouseDelta().y);
    input.mousePosition = sm::Vec2(GetMouseX(), GetMouseY());
}

void sge::InputProvider::operator()(Registry &registry, CommandBuffer &, sm::real)
{
    // Clear InputProvider's internal InputSnapshot
    this->m_curInput.clear();

    // Helper function to record events.
    updateInputSnapshot(this->m_curInput);

    // Query for player input resource.
    auto &playerInput = registry.getOrCreateResource<sge::PlayerInputResource>();
    
    // Update previous state to current state.
    playerInput.updatePrevious();

    // Update current state with new polled state.
    playerInput.updateCurrent(this->m_curInput);
}

const sge::InputActionSnapshot& sge::InputProvider::getCurrentSnapshot() const
{
    return this->m_curInput;
}

void sge::InputProvider::bindKey(int key, sge::Action action)
{
    this->m_keyToAction[key] = action;
}

void sge::InputProvider::bindMouse(int mouse, sge::Action action)
{
    this->m_mouseToAction[mouse] = action;
}