#ifndef SGE_ICOMPONENT_POOL_H
#define SGE_ICOMPONENT_POOL_H
#include <SGE/core/ecs/Entity.h>

namespace sge {
/**
 * Base class for ComponentPool classes (useful for storage).
 */
class IComponentPool {
protected:
    using RemoveFn = void(*)(IComponentPool*, Entity);
    using ClearFn = void(*)(IComponentPool*);

    IComponentPool(RemoveFn removeFunc, ClearFn clearFunc) :
    m_removeEntityFunc(removeFunc),
    m_clearFunc(clearFunc)
    {}

private:
    RemoveFn m_removeEntityFunc;
    ClearFn m_clearFunc;

public:
    ~IComponentPool() = default;

    void removeEntity(Entity e)
    {
        this->m_removeEntityFunc(this, e);
    }

    void clear()
    {
        this->m_clearFunc(this);
    }
};
}

#endif // SGE_COMPONENT_POOL_H