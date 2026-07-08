#ifndef SGE_CONSTRAINT_H
#define SGE_CONSTRAINT_H
#include <cstdint>

namespace sge {

enum class Constraint : uint32_t
{ INVALID = 0 };

static constexpr uint32_t FAKE_CONSTRAINT_FLAG = 0x80000000;
static constexpr uint32_t REAL_CONSTRAINT_MASK = 0x7FFFFFFF;

/**
 * Checks if a sge::Constraint is fake.
 * @param c the sge::Constraint ID
 * @return true/false if the ID is fake
 */
inline bool IsFakeConstraint(Constraint c)
{
    if (c == Constraint::INVALID) return true;

    return (static_cast<uint32_t>(c) & FAKE_CONSTRAINT_FLAG);
}

/**
 * Checks if a sge::Constraint is real.
 * @param c the sge::Constraint ID
 * @return true/false if the ID is real
 */
inline bool IsRealConstraint(Constraint c)
{
    return !IsFakeConstraint(c);
}

/**
 * @param c the sge::Constraint
 * @return the raw index of the sge::Constraint ID
 */
inline bool GetRawIndex(Constraint c)
{
    return static_cast<uint32_t>(c) & REAL_CONSTRAINT_MASK;
}

} // namespace sge

#endif // SGE_CONSTRAINT_H