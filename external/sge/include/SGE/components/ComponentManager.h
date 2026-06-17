#ifndef SGE_COMPONENT_MANAGER_H
#define SGE_COMPONENT_MANAGER_H
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <SGE/entities/Entity.h>
#include <SGE/components/IComponentPool.h>
#include <SGE/components/ComponentPool.h>

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
     * Checks if an entity has a specific component.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to check
     * @return true if the the entity has the component, false otherwise
     */
    template <typename T>
    bool has(Entity e);

    /**
     * Function that MUST be called just before an entity is destroyed.
     * @param e the entity that is going to be destroyed
     */
    void entityDestroyed(Entity e);

    /**
     * Helper function that returns a specific sge::Component pool type.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @return a pointer to the ComponentPool object
     */
    template <typename T>
    ComponentPool<T>* getPool();

    template <typename T>
    IComponentPool* getPoolInterface();
};
} // namespace sge

// Implementation

template <typename T>
inline T& sge::ComponentManager::addComponent(sge::Entity e, T&& component)
{
    return this->getPool<T>()->assign(e, std::forward<T>(component));
}

template <typename T>
inline void sge::ComponentManager::removeComponent(sge::Entity e)
{
    this->getPool<T>()->removeEntity(e);
}

template <typename T>
inline T& sge::ComponentManager::getComponent(Entity e)
{
    return this->getPool<T>()->get(e);
}

template <typename T>
inline bool sge::ComponentManager::has(Entity e)
{
    return this->getPool<T>()->has(e);
}

inline void sge::ComponentManager::entityDestroyed(Entity e)
{
    for (auto const &[typeIndex, pool] : this->m_pools)
    {
        pool->removeEntity(e);
    }
}

template<typename T>
inline sge::ComponentPool<T>* sge::ComponentManager::getPool()
{
    auto typeIndex = std::type_index(typeid(T));

    if (this->m_pools.find(typeIndex) == this->m_pools.end())
    {
        this->m_pools[typeIndex] = std::make_unique<sge::ComponentPool<T>>();
    }

    return static_cast<sge::ComponentPool<T>*>(this->m_pools.at(typeIndex).get());
}

template<typename T>
inline sge::IComponentPool* sge::ComponentManager::getPoolInterface()
{
    auto typeIndex = std::type_index(typeid(T));

    if (this->m_pools.find(typeIndex) == this->m_pools.end())
    {
        this->m_pools[typeIndex] = std::make_unique<sge::ComponentPool<T>>();
    }

    return this->m_pools.at(typeIndex).get();
}

#endif // SGE_COMPONENT_MANAGER_H