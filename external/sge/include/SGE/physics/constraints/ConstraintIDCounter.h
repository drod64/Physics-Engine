#ifndef SGE_CONSTRAINTS_ID_COUNTER_H
#define SGE_CONSTRAINTS_ID_COUNTER_H
#include <cstdint>

namespace sge {
    using ConstraintID = uint32_t;

    struct ConstraintIDCounter {
    private:
        static inline ConstraintID nextID()
        {
            static ConstraintID counter = 0;
            return counter++;
        }
    public:
        template <typename T>
        static ConstraintID get()
        {
            static const ConstraintID ID = nextID();
            return ID;
        }
    }; // struct ConstraintsIDCounter
} // namespace sge

#endif // SGE_CONSTRAINTS_ID_COUNTER_H