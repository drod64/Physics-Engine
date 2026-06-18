#ifndef SGE_ACTION_H
#define SGE_ACTION_H

namespace sge {
enum class Action : uint8_t {
    // Movement actions
    MoveForward = 0,
    MoveBackward,
    MoveRight,
    MoveLeft,
    Sprint,
    Crouch,

    // Interaction actions
    Jump,
    Interact,
    Cancel,

    // Execution actions
    PrimaryAction,
    SecondaryAction,
    Reload,

    // Global actions
    Pause,
    MenuToggle,

    // ADD DEBUG ACTIONS HERE

    // Count of actions
    MaxActions
};
} // namespace sge

#endif // SGE_ACTION_H