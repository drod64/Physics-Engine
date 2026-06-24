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
    void setMouseDelta(const sm::Vec2 &mouseDelta);
    sm::Vec2 getMouseDelta() const;

    void setMousePosition(const sm::Vec2 &mousePosition);
    sm::Vec2 getMousePosition() const;

    void setAction(uint32_t actionID, bool isDown);
    void updateCurrent(const std::bitset<512> &current);
    void prepareForPolling();
    
    bool consumeActionPressed(uint32_t actionID);
    bool isActionHeld(uint32_t actionID) const;
    bool consumeActionReleased(uint32_t actionID);

    template <typename EnumType>
    bool consumeActionPressed(EnumType actionID);

    template <typename EnumType>
    bool isActionHeld(EnumType actionID) const;

    template <typename EnumType>
    bool consumeActionReleased(EnumType actionID);
};
}

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

inline void sge::PlayerInputContext::updateCurrent(const std::bitset<512> &current)
{
    this->m_curHeld = current;
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