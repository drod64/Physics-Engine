#ifndef SGE_ACTION_H
#define SGE_ACTION_H
#include <cstdint>
#include <SGE/util/ActionType.h>

namespace sge {

using ActionID = uint32_t;

class Action {
private:
    ActionID m_id;
    ActionType m_type;

public:
    Action();
    Action(ActionID id, ActionType type);

    // Getters
    ActionID getID() const;
    ActionType getType() const;

    // Setters
    void setID(ActionID id);
    void setType(ActionType type);
};

} // namespace sge

#endif // SGE_ACTION_H