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
    sm::real movementAxisX = 0;
    sm::real movementAxisY = 0;
    sm::real movementAxisZ = 0;

    /**
     * Retrives the queued action events of the frame.
     * Use CPlayerController::getActionCount() to iterate through this array.
     * @return an array to the queued action events of the current frame
     */
    const ActionEvent* getActionEvents() const
    {
        return this->m_actions;
    }

    /**
     * @return the count of action events queued this frame.
     */
    size_t getActionCount() const
    {
        return this->m_curSize;
    }

    /**
     * Adds an action event to the queue.
     * @param actionEvent the action event to add
     */
    void addActionEvent(ActionEvent actionEvent)
    {
        if (this->m_curSize < sge::CPlayerController3::MAX_ACTIONS)
        {
            this->m_actions[this->m_curSize++] = actionEvent;
        }
    }

    /**
     * Clears the queue of action events.
     */
    void clearActions()
    {
        this->m_curSize = 0;
    }

    /**
     * Resets the all movement axes to 0.
     */
    void resetMovementAxes()
    {
        this->movementAxisX = 0;
        this->movementAxisY = 0;
        this->movementAxisZ = 0;
    }
};
}

#endif // SGE_CPLAYER_CONTROLLER_H