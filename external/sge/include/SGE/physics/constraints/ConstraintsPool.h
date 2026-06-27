#ifndef SGE_CONSTRAINTS_POOL_H
#define SGE_CONSTRAINTS_POOL_H
#include <vector>
#include <SGE/core/ecs/Entity.h>


namespace sge {
template <typename T>
class ConstraintsPool {
private:
    using ConstraintInstanceID = uint32_t;

    std::vector<T> dense;
    std::vector<> denseToEntity
}; 
} // namespace sge

#endif // SGE_CONSTRAINTS_POOL_H