#ifndef SGE_ICONSTRAINT_POOL_H
#define SGE_ICONSTRAINT_POOL_H
#include <SGE/physics/constraints/Constraint.h>

namespace sge {
class IConstraintPool {
protected:
    using RemoveFn = void(*)(IConstraintPool*, Constraint);
    using ClearFn = void(*)(IConstraintPool*);

    IConstraintPool(RemoveFn removeFunc, ClearFn clearFunc)
    {
        this->m_removeFunc = removeFunc;
        this->m_clearFunc = clearFunc;
    }

private:
    RemoveFn m_removeFunc;
    ClearFn m_clearFunc;

public:
    ~IConstraintPool() = default;

    void removeConstraint(Constraint c)
    {
        this->m_removeFunc(this, c);
    }

    void clear()
    {
        this->m_clearFunc(this);
    }

}; // class IConstraintPool
} // namespace sge

#endif // SGE_ICONSTRAINT_POOL_H