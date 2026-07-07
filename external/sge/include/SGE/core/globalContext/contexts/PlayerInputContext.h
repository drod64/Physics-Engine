#ifndef SGE_PLAYER_INPUT_CONTEXT_H
#define SGE_PLAYER_INPUT_CONTEXT_H
#include <cstdint>
#include <type_traits>
#include <cassert>
#include <bitset>
#include <SM/Vec2.h>

namespace sge {
class PlayerInputContext {
private:
    std::bitset<512> m_curHeld;
    std::bitset<512> m_prevHeld;

    std::bitset<512> m_justPressed;
    std::bitset<512> m_justReleased;

    sm::Vec2    m_mouseDelta;
    sm::Vec2    m_mousePosition;

public:
    /**
     * Set the mouse delta.
     * @param mouseDelta the mouse delta
     */
    void setMouseDelta(const sm::Vec2 &mouseDelta);

    /**
     * @return the mouse delta
     */
    sm::Vec2 getMouseDelta() const;

    /**
     * Set the mouse position.
     * @param mousePosition the mouse position
     */
    void setMousePosition(const sm::Vec2 &mousePosition);

    /**
     * @return the mouse position
     */
    sm::Vec2 getMousePosition() const;

    /**
     * Set the action state.
     * @param actionID the action represented as an uint32_t
     * @param isDown the action state (true/false)
     */
    void setAction(uint32_t actionID, bool isDown);

    /**
     * Prepare the PlayerInputContext for polling.
     * Use this at the start of the frame for input polling.
     */
    void prepareForPolling();
    
    /**
     * Consume an action that was just pressed.
     * @param actionID the action represented as an uint32_t
     * @return the action state (true/false)
     */
    bool consumeActionPressed(uint32_t actionID);

    /**
     * Check if an action is held.
     * @param actionID the action represented as an uint32_t
     * @return the action state (true/false)
     */
    bool isActionHeld(uint32_t actionID) const;

    /**
     * Consume an action that was just released.
     * @param actionID the action represented as an uint32_t
     * @return the action state (true/false)
     */
    bool consumeActionReleased(uint32_t actionID);

    /**
     * Consume an action that was just pressed.
     * @tparam EnumType the custom enum type
     * @param actionID the action represented as a custom enum.
     * 
     * NOTE: The function expects an enum type to be passed.
     * It then static_casts the enum to a uint32_t.
     * @return the action state (true/false)
     */
    template <typename EnumType>
    bool consumeActionPressed(EnumType actionID);

    /**
     * Check if an action is held.
     * @tparam EnumType the custom enum type
     * @param actionID the action represented as a custom enum.
     * 
     * NOTE: The function expects an enum type to be passed.
     * It then static_casts the enum to a uint32_t.
     * @return the action state (true/false)
     */
    template <typename EnumType>
    bool isActionHeld(EnumType actionID) const;

    /**
     * Consume an action that was just released.
     * @tparam EnumType the custom enum type
     * @param actionID the action represented as a custom enum.
     * 
     * NOTE: The function expects an enum type to be passed.
     * It then static_casts the enum to a uint32_t.
     * @return the action state (true/false)
     */
    template <typename EnumType>
    bool consumeActionReleased(EnumType actionID);
}; // class PlayerInputContext
} // namespace sge

// Implementation

inline void sge::PlayerInputContext::setMouseDelta(const sm::Vec2 &mouseDelta)
{
    this->m_mouseDelta = mouseDelta;
}

inline sm::Vec2 sge::PlayerInputContext::getMouseDelta() const
{
    return this->m_mouseDelta;
}

inline void sge::PlayerInputContext::setMousePosition(const sm::Vec2 &mousePosition)
{
    this->m_mousePosition = mousePosition;
}

inline sm::Vec2 sge::PlayerInputContext::getMousePosition() const
{
    return this->m_mousePosition;
}

inline void sge::PlayerInputContext::setAction(uint32_t actionID, bool isDown)
{
    if ((actionID) > m_curHeld.size()) return;
    
    if (isDown)
    {
        // Set continous snapshot.
        this->m_curHeld.set(actionID, true);

        // Mark just pressed (sticky) snapshot if in the previous frame key was not down.
        if (!this->m_prevHeld.test(actionID))
        {
            this->m_justPressed.set(actionID, true);
        }
    }
    else
    {
        // Key is down.
        // Key was released if it was held last frame and not this frame.
        if (this->m_prevHeld.test(actionID))
        {
            this->m_justReleased.set(actionID, true);
        }
    }
}

inline void sge::PlayerInputContext::prepareForPolling()
{
    this->m_prevHeld = this->m_curHeld;
    this->m_curHeld.reset();
}

inline bool sge::PlayerInputContext::consumeActionPressed(uint32_t actionID)
{
    if (this->m_justPressed.test(actionID))
    {
        this->m_justPressed.reset(actionID);
        return true;
    }

    return false;
}

inline bool sge::PlayerInputContext::isActionHeld(uint32_t actionID) const
{
    return this->m_prevHeld.test(actionID) && this->m_curHeld.test(actionID);
}

inline bool sge::PlayerInputContext::consumeActionReleased(uint32_t actionID)
{
    if (this->m_justReleased.test(actionID))
    {
        this->m_justReleased.reset(actionID);
        return true;
    }

    return false;
}

template <typename EnumType>
inline bool sge::PlayerInputContext::consumeActionPressed(EnumType actionID)
{
    static_assert(std::is_enum_v<EnumType>, "[PlayerInputContext]: Parameter must be an enum type!");
    
    return this->consumeActionPressed(static_cast<uint32_t>(actionID));
}

template <typename EnumType>
inline bool sge::PlayerInputContext::isActionHeld(EnumType actionID) const
{
    static_assert(std::is_enum_v<EnumType>, "[PlayerInputContext]: Parameter must be an enum type!");
    
    return this->isActionHeld(static_cast<uint32_t>(actionID));
}

template <typename EnumType>
inline bool sge::PlayerInputContext::consumeActionReleased(EnumType actionID)
{
    static_assert(std::is_enum_v<EnumType>, "[PlayerInputContext]: Parameter must be an enum type!");
    
    return this->consumeActionReleased(static_cast<uint32_t>(actionID));
}

#endif // SGE_PLAYER_INPUT_CONTEXT_H