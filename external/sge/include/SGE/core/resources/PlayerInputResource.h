#ifndef SGE_PLAYER_INPUT_RESOURCE_H
#define SGE_PLAYER_INPUT_RESOURCE_H
#include <SGE/core/InputActionSnapshot.h>
#include <SGE/util/Action.h>

namespace sge {
class PlayerInputResource {
private:
    InputActionSnapshot m_curInput;
    InputActionSnapshot m_prevInput;

public:
    void updateCurrent(const InputActionSnapshot &current);
    void updatePrevious();
    
    bool isActionPressed(Action action) const;
    bool isActionHeld(Action action) const;
    bool isActionReleased(Action action) const;
};
}

#endif // SGE_PLAYER_INPUT_RESOURCE_H