#ifndef SGE_PLAYER_INPUT_RESOURCE_H
#define SGE_PLAYER_INPUT_RESOURCE_H
#include <cstdint>
#include <type_traits>
#include <cassert>
#include <SM/Vec2.h>
#include <SGE/managers/InputActionSnapshot.h>

namespace sge {
class PlayerInputResource {
private:
    InputActionSnapshot m_curInput;
    InputActionSnapshot m_prevInput;

public:
    void setMouseDelta(const sm::Vec2 &mouseDelta);
    sm::Vec2 getMouseDelta() const;

    void setMousePosition(const sm::Vec2 &mousePosition);
    sm::Vec2 getMousePosition() const;

    void setAction(uint32_t actionID, bool value);
    void updateCurrent(const InputActionSnapshot &current);
    void updatePrevious();
    void reset();
    
    bool isActionPressed(uint32_t actionID) const;
    bool isActionHeld(uint32_t actionID) const;
    bool isActionReleased(uint32_t actionID) const;

    template <typename EnumType>
    bool isActionPressed(EnumType actionID) const;

    template <typename EnumType>
    bool isActionHeld(EnumType actionID) const;

    template <typename EnumType>
    bool isActionReleased(EnumType actionID) const;
};
}

// Implementation

inline void sge::PlayerInputResource::setMouseDelta(const sm::Vec2 &mouseDelta)
{
    this->m_curInput.mouseDelta = mouseDelta;
}

inline sm::Vec2 sge::PlayerInputResource::getMouseDelta() const
{
    return this->m_curInput.mouseDelta;
}

inline void sge::PlayerInputResource::setMousePosition(const sm::Vec2 &mousePosition)
{
    this->m_curInput.mousePosition = mousePosition;
}

inline sm::Vec2 sge::PlayerInputResource::getMousePosition() const
{
    return this->m_curInput.mousePosition;
}

inline void sge::PlayerInputResource::setAction(uint32_t actionID, bool value)
{
    this->m_curInput.setAction(actionID, value);
}

inline void sge::PlayerInputResource::updateCurrent(const sge::InputActionSnapshot &current)
{
    this->m_curInput = current;
}

inline void sge::PlayerInputResource::updatePrevious()
{
    this->m_prevInput = this->m_curInput;
}

inline void sge::PlayerInputResource::reset()
{
    this->m_curInput.reset();
}

inline bool sge::PlayerInputResource::isActionPressed(uint32_t actionID) const
{
    return !this->m_prevInput.getActionState(actionID) && this->m_curInput.getActionState(actionID);
}

inline bool sge::PlayerInputResource::isActionHeld(uint32_t actionID) const
{
    return this->m_prevInput.getActionState(actionID) && this->m_curInput.getActionState(actionID);
}

inline bool sge::PlayerInputResource::isActionReleased(uint32_t actionID) const
{
    return this->m_prevInput.getActionState(actionID) && !this->m_curInput.getActionState(actionID);
}

template <typename EnumType>
inline bool sge::PlayerInputResource::isActionPressed(EnumType actionID) const
{
    static_assert(std::is_enum_v<EnumType>, "[PlayerInputResource]: Parameter must be an enum type!");
    
    return this->isActionPressed(static_cast<uint32_t>(actionID));
}

template <typename EnumType>
inline bool sge::PlayerInputResource::isActionHeld(EnumType actionID) const
{
    static_assert(std::is_enum_v<EnumType>, "[PlayerInputResource]: Parameter must be an enum type!");
    
    return this->isActionHeld(static_cast<uint32_t>(actionID));
}

template <typename EnumType>
inline bool sge::PlayerInputResource::isActionReleased(EnumType actionID) const
{
    static_assert(std::is_enum_v<EnumType>, "[PlayerInputResource]: Parameter must be an enum type!");
    
    return this->isActionReleased(static_cast<uint32_t>(actionID));
}

#endif // SGE_PLAYER_INPUT_RESOURCE_H