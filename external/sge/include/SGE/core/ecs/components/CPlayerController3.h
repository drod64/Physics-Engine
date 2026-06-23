#ifndef SGE_CPLAYER_CONTROLLER_H
#define SGE_CPLAYER_CONTROLLER_H
#include <array>
#include <vector>
#include <cstdint>
#include <cassert>
#include <type_traits>
#include <SM/Precision.h>
#include <SGE/core/ecs/ActionEvent.h>

namespace sge {
class CPlayerController3 {
private:
    std::vector<ActionEvent> m_actions;

public:
    sm::real movementAxisX;
    sm::real movementAxisY;
    sm::real movementAxisZ;

    CPlayerController3()
    {
        this->movementAxisX = 0;
        this->movementAxisY = 0;
        this->movementAxisZ = 0;
    }

    const std::vector<ActionEvent>& getActionEvents() const
    {
        return this->m_actions;
    }

    void addActionEvent(ActionEvent actionEvent)
    {
        this->m_actions.push_back(actionEvent);
    }

    void clearActions()
    {
        this->m_actions.clear();
    }

    void resetMovementAxes()
    {
        this->movementAxisX = 0;
        this->movementAxisY = 0;
        this->movementAxisZ = 0;
    }
};
}

#endif // SGE_CPLAYER_CONTROLLER_H