#ifndef SGE_CONSTRAINT_POOL_MANAGER_H
#define SGE_CONSTRAINT_POOL_MANAGER_H
#include <memory>
#include <vector>
#include <SGE/physics/constraints/ConstraintIDCounter.h>
#include <SGE/physics/constraints/ConstraintManager.h>
#include <SGE/physics/constraints/ConstraintPool.h>

namespace sge {
class ConstraintPoolManager {
private:
    ConstraintManager m_manager;
    std::vector<std::unique_ptr<IConstraintPool>> m_pools;

public:
    ConstraintPoolManager() = default;
    ~ConstraintPoolManager() = default;

    ConstraintPoolManager(const ConstraintPoolManager &) = delete;
    ConstraintPoolManager& operator= (const ConstraintPoolManager &) = delete;

    template <typename T, typename... Args>
    Constraint addConstraint(Args&&... args);

    template <typename T>
    T& getConstraint(Constraint c);

    template <typename T>
    const T& getConstraint(Constraint c) const;

    template <typename T>
    bool has(Constraint c) const;
    
    void constraintDestroyed(Constraint c);

    void clear();

    template <typename T>
    ConstraintPool<T>* getOrCreatePool();

    template <typename T>
    const ConstraintPool<T>* getPool() const;
}; // class ConstraintPoolManager
} // namespace sge

// Implementation

template <typename T, typename... Args>
inline sge::Constraint sge::ConstraintPoolManager::addConstraint(Args&&... args)
{
    sge::Constraint id = this->m_manager.createConstraint();

    this->getOrCreatePool<T>()->addConstraint(id, std::forward<Args>(args)...);

    return id;
}

template <typename T>
inline T& sge::ConstraintPoolManager::getConstraint(sge::Constraint c)
{
    this->getOrCreatePool<T>()->getConstraint(c);
}

template <typename T>
inline const T& sge::ConstraintPoolManager::getConstraint(sge::Constraint c) const
{
    const sge::ConstraintPool<T> *pool = this->getPool<T>();

    assert(pool && "Pool does not exist!");

    return pool->getConstraint(c);
}

template <typename T>
inline bool sge::ConstraintPoolManager::has(sge::Constraint c) const
{
    const sge::ConstraintPool<T> *pool = this->getPool<T>();

    if (pool)
    {
        return pool->has(c);
    }

    return false;
}

inline void sge::ConstraintPoolManager::constraintDestroyed(sge::Constraint c)
{
    // TODO Optimize so it doesn't waste CPU cycles looping through all pools.
    // ...just to delete one constraint.
    for (auto &pool : this->m_pools)
    {
        pool->removeConstraint(c);
    }
}

inline void sge::ConstraintPoolManager::clear()
{
    this->m_pools.clear();
}

template <typename T>
inline sge::ConstraintPool<T>* sge::ConstraintPoolManager::getOrCreatePool()
{
    sge::ConstraintID id = sge::ConstraintIDCounter::get<T>();

    if (id >= this->m_pools.size())
    {
        size_t newSize = this->m_pools.size() * 2;
        if(newSize <= id) newSize = id + 1;

        this->m_pools.resize(newSize);
    }

    if (!this->m_pools[id])
    {
        this->m_pools[id] = std::make_unique<sge::ConstraintPool<T>>();
    }

    return static_cast<sge::ConstraintPool<T>*>(this->m_pools[id].get());
}

template <typename T>
inline const sge::ConstraintPool<T>* sge::ConstraintPoolManager::getPool() const
{
    sge::ConstraintID id = sge::ConstraintIDCounter::get<T>();

    if (id >= this->m_pools.size()) return nullptr;

    return static_cast<sge::ConstraintPool<T>*>(this->m_pools[id]);
}


#endif // SGE_CONSTRAINT_POOL_MANAGER_H