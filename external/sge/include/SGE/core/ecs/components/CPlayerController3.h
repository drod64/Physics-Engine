#ifndef SGE_CPLAYER_CONTROLLER_H
#define SGE_CPLAYER_CONTROLLER_H
#include <cstdint>
#include <SM/Precision.h>
#include <SGE/core/ecs/ActionEvent.h>

namespace sge {
class CPlayerController3 {
private:
    static const size_t MAX_ACTIONS = 128;

    ActionEvent m_actions[MAX_ACTIONS];
    size_t m_curSize = 0;


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

    const ActionEvent* getActionEvents() const
    {
        return this->m_actions;
    }

    size_t getActionCount() const
    {
        return this->m_curSize;
    }

    void addActionEvent(ActionEvent actionEvent)
    {
        if (this->m_curSize < sge::CPlayerController3::MAX_ACTIONS)
        {
            this->m_actions[this->m_curSize++] = actionEvent;
        }
    }

    void clearActions()
    {
        this->m_curSize = 0;
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