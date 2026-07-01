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

    /**
     * Creates a new constraint ID based on index and generation.
     * @param index the index of the ID
     * @param generation the generation of the ID
     * @return the new sge::Constraint ID
     */
    Constraint createConstraintID(uint32_t index, uint32_t generation) const;

public:
    /**
     * Default constructor.
     */
    ConstraintManager();

    /**
     * @return the index of a sge::Constraint
     */
    uint32_t getIndex(Constraint c) const;

    /**
     * @return the generation of a sge::Constraint
     */
    uint32_t getGeneration(Constraint c) const;

    /**
     * @return the new sge::Constraint ID handle
     */
    Constraint createConstraint();

    /**
     * Destroys a sge::Constraint ID handle.
     * @param c the sge::Constraint to destroy
     */
    void destroyConstraint(Constraint c);

    /**
     * Checks if a sge::Constraint ID is valid.
     * @param c the sge::Constraint to check
     */
    bool isValid(Constraint c) const;
}; // class ConstraintManager
} // namespace sge

#endif // SGE_CONSTRAINT_MANAGER_H