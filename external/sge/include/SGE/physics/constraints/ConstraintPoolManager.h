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

    /**
     * Adds a constraint.
     * @tparam T the constraint type
     * @tparam U lvalue/rvalue of the constraint type
     * @param constraintData the constraint object
     */
    template <typename T, typename U>
    Constraint addConstraint(U &&constraintData);

    /**
     * @param c the sge::Constraint ID handle
     * @returns a transient reference to the constraint object
     */
    template <typename T>
    T& getConstraint(Constraint c);

    /**
     * @param c the sge::Constraint ID handle
     * @returns a transient reference to the constraint object
     */
    template <typename T>
    const T& getConstraint(Constraint c) const;

    /**
     * Checks if a sge::Constraint ID handle has a constraint type.
     * @param c the sge::Constraint handle
     * @return true/false whether the handle contains the type
     */
    template <typename T>
    bool has(Constraint c) const;
    
    /**
     * Destroys a constraint.
     * @param c the sge::Constraint ID handle
     */
    void constraintDestroyed(Constraint c);

    /**
     * Clears all pools.
     */
    void clear();

    /**
     * Retrieves or creates a constraint pool.
     * @return the constraint pool pointer (will never return nullptr)
     */
    template <typename T>
    ConstraintPool<T>* getOrCreatePool();

    /**
     * Retrieves a constraint pool.
     * @return the constraint pool const pointer (WILL return nullptr if pool does not exist)
     */
    template <typename T>
    const ConstraintPool<T>* getPool() const;
}; // class ConstraintPoolManager
} // namespace sge

// Implementation

template <typename T, typename U>
inline sge::Constraint sge::ConstraintPoolManager::addConstraint(U &&constraintData)
{
    using CleanType = std::remove_cvref_t<T>;

    sge::Constraint id = this->m_manager.createConstraint();

    this->getOrCreatePool<T>()->addConstraint(id, std::forward<U>(constraintData));

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
        if (pool)
        {
            pool->removeConstraint(c);
        }
    }

    // Recycle ID
    this->m_manager.destroyConstraint(c);
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