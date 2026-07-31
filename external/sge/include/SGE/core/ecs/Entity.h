#ifndef SGE_ENTITY_H
#define SGE_ENTITY_H
#include <cstdint>

namespace sge {

enum class Entity : uint32_t
{INVALID = 0};

// Bit 31 mask (1 in the highest bit, 0 everywhere else)
static constexpr uint32_t FAKE_ENTITY_FLAG = 0x80000000;
static constexpr uint32_t PURE_INDEX_MASK  = 0x000FFFFF;

static constexpr uint32_t GENERATION_MASK  = 0x7FF00000;
static constexpr uint32_t GENERATION_SHIFT = 20;

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
    return static_cast<uint32_t>(e) & PURE_INDEX_MASK;
}

inline uint32_t GetGeneration(Entity e)
{
    return (static_cast<uint32_t>(e) & GENERATION_MASK) >> GENERATION_SHIFT;
}

} // namespace sge

#endif // SGE_ENTITY_H