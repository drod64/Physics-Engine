#ifndef SGE_CONSTRAINT_MANAGER_H
#define SGE_CONSTRAINT_MANAGER_H
#include <vector>
#include <cstdint>
#include <queue>
#include <SGE/physics/constraints/Constraint.h>

namespace sge {
class ConstraintManager {
private:
    static const uint32_t INDEX_MASK;
    static const uint32_t MIN_GENERATION;

    std::vector<uint32_t> m_generations;
    std::queue<uint32_t> m_freeIndices;

    Constraint createConstraintID(uint32_t index, uint32_t generation) const;

public:
    ConstraintManager();

    uint32_t getIndex(Constraint c) const;

    uint32_t getGeneration(Constraint c) const;

    Constraint createConstraint();

    void destroyConstraint(Constraint c);

    bool isValid(Constraint c) const;
}; // class ConstraintManager
} // namespace sge

#endif // SGE_CONSTRAINT_MANAGER_H