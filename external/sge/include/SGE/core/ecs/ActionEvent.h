#ifndef SGE_ACTION_EVENT_H
#define SGE_ACTION_EVENT_H
#include <cstdint>

namespace sge {
enum class ActionType : uint16_t {
    Pressed,
    Released,

    NOTHING
};

struct ActionEvent {
    uint32_t actionID;
    ActionType type;

    ActionEvent() :
    ActionEvent(0, ActionType::NOTHING)
    {}

    ActionEvent(uint32_t actionID, ActionType type)
    {
        this->actionID = actionID;
        this->type = type;
    }
};
}

#endif // SGE_ACTION_EVENT_H