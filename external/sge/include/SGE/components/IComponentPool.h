#ifndef SGE_ICOMPONENT_POOL_H
#define SGE_ICOMPONENT_POOL_H
#include <SGE/entities/Entity.h>

namespace sge {
/**
 * Base class for ComponentPool classes. Sub-classes must implement the removeEntity() function.
 */
class IComponentPool {
public:
    /**
     * Pure virtual function that must be implemented by a sub-class.
     */
    virtual void removeEntity(Entity e) = 0;
    virtual bool has(Entity e) const = 0;

    virtual size_t size() const = 0;
    virtual Entity getEntityAt(size_t index) const = 0;
};
}

#endif // SGE_COMPONENT_POOL_H