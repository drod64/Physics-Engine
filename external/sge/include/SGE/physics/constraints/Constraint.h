#ifndef SGE_CONSTRAINT_H
#define SGE_CONSTRAINT_H
#include <cstdint>

namespace sge {

enum class Constraint : uint32_t
{ INVALID = 0 };

static constexpr uint32_t FAKE_CONSTRAINT_FLAG = 0x80000000;
static constexpr uint32_t REAL_CONSTRAINT_MASK = 0x7FFFFFFF;

inline bool IsFakeConstraint(Constraint c)
{
    if (c == Constraint::INVALID) return true;

    return (static_cast<uint32_t>(c) & FAKE_CONSTRAINT_FLAG);
}

inline bool IsRealConstraint(Constraint c)
{
    return !IsFakeConstraint(c);
}

inline bool GetRawIndex(Constraint c)
{
    return static_cast<uint32_t>(c) & REAL_CONSTRAINT_MASK;
}

} // namespace sge

#endif // SGE_CONSTRAINT_H