#ifndef SGE_COMPONENT_MANAGER_H
#define SGE_COMPONENT_MANAGER_H
#include <memory>
#include <cstddef>
#include <vector>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/ComponentPool.h>
#include <SGE/managers/IComponentPool.h>
#include <SGE/core/ecs/components/ComponentIDCounter.h>

namespace sge {
/**
 * Data structure class that helps manage a collection of different types of sge::Components.
 * Components can be added, removed, and retrieved.
 * All you need to pass in is the sge:Entity enum (which essentially is just an ID).
 */
class ComponentManager {
private:
    std::vector<std::unique_ptr<IComponentPool>> m_pools;

public:
    ComponentManager() = default;

    ~ComponentManager() = default;

    ComponentManager(const ComponentManager &) = delete;
    
    ComponentManager& operator= (const ComponentManager &) = delete;

    /**
     * Adds a component to a specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the Entity (ID) to assign it to 
     * @param component the component to add
     * @return a reference to the newly added component
     */
    template <typename T, typename U>
    T& addComponent(Entity e, U &&component);

    /**
     * Removes a component from an entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the Entity to remove it from
     */
    template <typename T>
    void removeComponent(Entity e);

    /**
     * Retrieves a component from the specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to retrieve the component from
     * @return a reference to the component.
     */
    template <typename T>
    T& getComponent(Entity e);

    /**
     * Retrieves a component from the specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to retrieve the component from
     * @return a reference to the component.
     */
    template <typename T>
    const T& getComponent(Entity e) const;

    /**
     * Checks if an entity has a specific component.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to check
     * @return true if the the entity has the component, false otherwise
     */
    template <typename T>
    bool has(Entity e) const;

    /**
     * Function that MUST be called just before an entity is destroyed.
     * @param e the entity that is going to be destroyed
     */
    void entityDestroyed(Entity e);

    /**
     * Clears all component pools from the ComponentManager.
     */
    void clear();

    /**
     * Helper function that returns a specific sge::Component pool type.
     * If a pool type does not exist, it creates it.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @return a pointer to the existing or newly created ComponentPool<T> object
     */
    template <typename T>
    ComponentPool<T>* getOrCreatePool();

    /**
     * Helper function that returns a specific sge::Component pool type.
     * If pool type does not exists, it returns nullptr.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @return a pointer to the ComponentPool<T> object OR nullptr if pool does not exists
     */
    template <typename T>
    const ComponentPool<T>* getPool() const;
};
} // namespace sge

// Implementation

template <typename T, typename U>
inline T& sge::ComponentManager::addComponent(sge::Entity e, U &&component)
{
    using CleanType = std::decay_t<T>;
    return this->getOrCreatePool<CleanType>()->assign(e, std::forward<U>(component));
}

template <typename T>
inline void sge::ComponentManager::removeComponent(sge::Entity e)
{
    this->getOrCreatePool<T>()->concreteRemoveEntity(e);
}

template <typename T>
inline T& sge::ComponentManager::getComponent(Entity e)
{
    return this->getOrCreatePool<T>()->get(e);
}

template <typename T>
inline const T& sge::ComponentManager::getComponent(Entity e) const
{
    const sge::ComponentPool<T> *pool = this->getPool<T>();

    assert(pool && "Component pool does not exist!");

    return pool->get(e);
}

template <typename T>
inline bool sge::ComponentManager::has(Entity e) const
{
    const sge::ComponentPool<T> *pool = this->getPool<T>();

    // Check if pool exists.
    if (pool)
    {
        return pool->has(e);
    }

    return false;
}

inline void sge::ComponentManager::entityDestroyed(Entity e)
{
    for (auto &pool : this->m_pools)
    {
        pool->removeEntity(e);
    }
}

inline void sge::ComponentManager::clear()
{
    this->m_pools.clear();
}

template<typename T>
inline sge::ComponentPool<T>* sge::ComponentManager::getOrCreatePool()
{
    sge::ComponentID id = sge::ComponentIDCounter::get<T>();

    // Resize pool storage if necessary.
    if (id >= this->m_pools.size())
    {
        size_t newSize = this->m_pools.size() * 2;
        if (newSize <= id) newSize = id + 1;

        this->m_pools.resize(newSize);
    }

    // Initialize pool if necessary.
    if (!this->m_pools[id])
    {
        this->m_pools[id] = std::make_unique<sge::ComponentPool<T>>();
    }

    // Return existing/new pool.
    return static_cast<sge::ComponentPool<T>*>(this->m_pools[id].get());
}

template<typename T>
inline const sge::ComponentPool<T>* sge::ComponentManager::getPool() const
{
    sge::ComponentID id = sge::ComponentIDCounter::get<T>();

    // Return nullptr if id exceeds pool storage size.
    if (id >= this->m_pools.size()) return nullptr;

    // Return pool (may be nullptr if it hasn't been initialized).
    return static_cast<const sge::ComponentPool<T>*>(this->m_pools[id].get());
}

#endif // SGE_COMPONENT_MANAGER_H