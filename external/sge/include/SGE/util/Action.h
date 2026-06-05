#ifndef SGE_ACTION_H
#define SGE_ACTION_H
#include <cstdint>
#include <SGE/util/ActionType.h>

namespace sge {

using ActionID = uint32_t;
/**
 * Stores data related to a specific action such as its ID and type.
 */
class Action {
private:
    ActionID m_id;
    ActionType m_type;

public:
    /**
     * Default Constructor.
     */
    Action();

    /**
     * Parameterized Constructor.
     * @param id the ID of the action
     * @param type the type of action
     */
    Action(ActionID id, ActionType type);

    /**
     * Retrieves the action ID.
     * @return the ID of the action
     */
    ActionID getID() const;

    /**
     * Retrives the action type.
     * @return the type of action
     */
    ActionType getType() const;

    /**
     * Sets the action ID.
     * @param id the ID of the action
     */
    void setID(ActionID id);

    /**
     * Sets the action type.
     * @param type the type of action
     */
    void setType(ActionType type);
};

} // namespace sge

#endif // SGE_ACTION_H