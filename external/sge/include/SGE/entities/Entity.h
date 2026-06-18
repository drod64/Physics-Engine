#ifndef SGE_ENTITY_H
#define SGE_ENTITY_H
#include <cstdint>

namespace sge {

enum class Entity : uint32_t
{INVALID = 0};

// Bit 31 mask (1 in the highest bit, 0 everywhere else)
static constexpr uint32_t FAKE_ENTITY_FLAG = 0x80000000;
static constexpr uint32_t REAL_ENTITY_MASK = 0x7FFFFFFF;

/**
 * Checks if an Entity is fake.
 * @param e the entity to check
 * @return true if fake, false otherwise
 */
inline bool IsFakeEntity(Entity e)
{   if (e == Entity::INVALID) return true;
    
    return (static_cast<uint32_t>(e) & FAKE_ENTITY_FLAG);
}

/**
 * Checks if an Entity is real.
 * @param e the entity to check
 * @return true if real, false otherwise
 */
inline bool IsRealEntity(Entity e)
{
    if (e == Entity::INVALID) return false;

    return !(static_cast<uint32_t>(e) & FAKE_ENTITY_FLAG);
}

/**
 * Retrives the raw index of the Entity.
 * @param e the Entity to get the index from
 * @return the raw index of the Entity.
 */
inline uint32_t GetRawIndex(Entity e)
{
    return static_cast<uint32_t>(e) & REAL_ENTITY_MASK;
}

} // namespace sge

#endif // SGE_ENTITY_H