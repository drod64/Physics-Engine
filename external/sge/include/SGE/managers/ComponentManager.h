#ifndef SGE_COMPONENT_MANAGER_H
#define SGE_COMPONENT_MANAGER_H
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/IComponentPool.h>
#include <SGE/managers/ComponentPool.h>

namespace sge {
/**
 * Data structure class that helps manage a collection of different types of sge::Components.
 * Components can be added, removed, and retrieved.
 * All you need to pass in is the sge:Entity enum (which essentially is just an ID).
 */
class ComponentManager {
private:
    /**
     * Holds the collection of sge::Components.
     * Each different type of sge::Component gets their own pool.
     */
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pools;

public:
    /**
     * Adds a component to a specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the Entity (ID) to assign it to 
     * @param component the component to add
     * @return a reference to the newly added component
     */
    template <typename T>
    T& addComponent(Entity e, T&& component);

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

    /**
     * Returns the base pointer (sge::IComponent) of a sge::ComponentPool<T>.
     * If the type does not exist, it creates it.
     * @return a pointer to the existing or newly created base type of the ComponentPool<T>
     */
    template <typename T>
    IComponentPool* getOrCreatePoolInterface();

    /**
     * Returns the the base pointer (sge::IComponent) of a sge::ComponentPool<T>/
     * If the the type DOES NOT EXIST, it returns nullptr.
     * @return a pointer to the existing base type of ComponentPool<T>, if it DOES NOT EXIST, it returns nullptr
     */
    template <typename T>
    const IComponentPool* getPoolInterface() const;
};
} // namespace sge

// Implementation

template <typename T>
inline T& sge::ComponentManager::addComponent(sge::Entity e, T&& component)
{
    return this->getOrCreatePool<T>()->assign(e, std::forward<T>(component));
}

template <typename T>
inline void sge::ComponentManager::removeComponent(sge::Entity e)
{
    this->getOrCreatePool<T>()->removeEntity(e);
}

template <typename T>
inline T& sge::ComponentManager::getComponent(Entity e)
{
    return this->getOrCreatePool<T>()->get(e);
}

template <typename T>
inline const T& sge::ComponentManager::getComponent(Entity e) const
{
    auto pool = this->getPool<T>();

    assert(pool && "Component pool does not exist!");

    return pool->get(e);
}

template <typename T>
inline bool sge::ComponentManager::has(Entity e) const
{
    auto pool = this->getPool<T>();

    // Check if pool exists.
    if (pool)
    {
        return pool->has(e);
    }

    return false;
}

inline void sge::ComponentManager::entityDestroyed(Entity e)
{
    for (auto const &[typeIndex, pool] : this->m_pools)
    {
        pool->removeEntity(e);
    }
}

template<typename T>
inline sge::ComponentPool<T>* sge::ComponentManager::getOrCreatePool()
{
    auto typeIndex = std::type_index(typeid(T));

    if (this->m_pools.find(typeIndex) == this->m_pools.end())
    {
        this->m_pools[typeIndex] = std::make_unique<sge::ComponentPool<T>>();
    }

    return static_cast<sge::ComponentPool<T>*>(this->m_pools.at(typeIndex).get());
}

template<typename T>
inline const sge::ComponentPool<T>* sge::ComponentManager::getPool() const
{
    auto typeIndex = std::type_index(typeid(T));

    if (this->m_pools.find(typeIndex) == this->m_pools.end())
    {
        return nullptr;
    }

    return static_cast<sge::ComponentPool<T>*>(this->m_pools.at(typeIndex).get());
}

template<typename T>
inline sge::IComponentPool* sge::ComponentManager::getOrCreatePoolInterface()
{
    auto typeIndex = std::type_index(typeid(T));

    if (this->m_pools.find(typeIndex) == this->m_pools.end())
    {
        this->m_pools[typeIndex] = std::make_unique<sge::ComponentPool<T>>();
    }

    return this->m_pools.at(typeIndex).get();
}

template <typename T>
inline const sge::IComponentPool* sge::ComponentManager::getPoolInterface() const
{
    auto typeIndex = std::type_index(typeid(T));

    if (this->m_pools.find(typeIndex) == this->m_pools.end())
    {
        return nullptr;
    }

    return this->m_pools.at(typeIndex).get();
}

#endif // SGE_COMPONENT_MANAGER_H