#ifndef SGE_COMPONENT_MANAGER_H
#define SGE_COMPONENT_MANAGER_H
#include <memory>
#include <cstddef>
#include <vector>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/ComponentPool.h>
#include <SGE/core/ecs/components/ComponentIDCounter.h>

namespace sge {
/**
 * Data structure class that helps manage a collection of different types of sge::Components.
 * Components can be added, removed, and retrieved.
 * All you need to pass in is the sge:Entity enum (which essentially is just an ID).
 */
class ComponentManager {
private:
    using UniformPoolFootPrint = sge::ComponentPool<char>;
    static constexpr size_t POOL_SIZE_BYTES = sizeof (UniformPoolFootPrint);

    // Raw bytes to store ComponentPools in.
    std::vector<std::byte> m_arena;
    // Tracks initialized pools.
    std::vector<bool> m_poolInitialized;
    // Tracks destructor of each pool.
    std::vector<void(*)(void*)> m_destructors;

public:
    ComponentManager() = default;

    ~ComponentManager();

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
    T& addComponent(Entity e, U&& component);

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

inline sge::ComponentManager::~ComponentManager()
{
    for (size_t i = 0; i < this->m_poolInitialized.size(); ++i)
    {
        if (this->m_poolInitialized[i] && this->m_destructors[i])
        {
            std::byte *poolLocation = &this->m_arena[i * POOL_SIZE_BYTES];
            this->m_destructors[i](poolLocation);
        }
    }
}

// T = raw class type
// U = reference parameter
template <typename T, typename U> 
inline T& sge::ComponentManager::addComponent(sge::Entity e, U&& component)
{
    return this->getOrCreatePool<T>()->assign(e, std::forward<U>(component));
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
    size_t activePools = this->m_poolInitialized.size();

    for (size_t i = 0; i < activePools; ++i)
    {
        if (this->m_poolInitialized[i])
        {
            sge::IComponentPool *pool = reinterpret_cast<sge::IComponentPool*>(&this->m_arena[i * POOL_SIZE_BYTES]);
            pool->removeEntity(e);
        }
    }
}

inline void sge::ComponentManager::clear()
{
    size_t activePools = this->m_poolInitialized.size();

    for (size_t i = 0; i < activePools; ++i)
    {
        if (this->m_poolInitialized[i])
        {
            sge::IComponentPool *pool = reinterpret_cast<sge::IComponentPool*>(&this->m_arena[i * POOL_SIZE_BYTES]);
            pool->clear();
        }
    }
}

template<typename T>
inline sge::ComponentPool<T>* sge::ComponentManager::getOrCreatePool()
{
    sge::ComponentID id = sge::ComponentIDCounter::get<T>();

    // Create new pool slots if necessary.
    if (id >= this->m_poolInitialized.size())
    {
        size_t newPoolCount = this->m_poolInitialized.size() * 2;
        if (newPoolCount <= id) newPoolCount = id + 1;

        // Resize arena.
        this->m_arena.resize(newPoolCount * POOL_SIZE_BYTES);
        // Resize initialization tracker.
        this->m_poolInitialized.resize(newPoolCount, false);
        // Resize destructor tracker.
        this->m_destructors.resize(newPoolCount, nullptr);
    }

    // Calculate location of pool.
    std::byte *poolLocation = &this->m_arena[id * POOL_SIZE_BYTES];

    // Initialize new pool.
    if (!this->m_poolInitialized[id])
    {
        new(poolLocation) sge::ComponentPool<T>();
        // Mark pool as initialized.
        this->m_poolInitialized[id] = true;
        // Store callback for destructor.
        this->m_destructors[id] = [](void* ptr) {
            static_cast<sge::ComponentPool<T>*>(ptr)->~ComponentPool<T>();
        };
    }

    // Return existing or newly created pool.
    return reinterpret_cast<sge::ComponentPool<T>*>(poolLocation);
}

template<typename T>
inline const sge::ComponentPool<T>* sge::ComponentManager::getPool() const
{
    sge::ComponentID id = sge::ComponentIDCounter::get<T>();

    // Pool either is out of bounds or was never initialized.
    if (id >= this->m_poolInitialized.size() || !this->m_poolInitialized.at(id)) return nullptr;

    // Calculate pool location.
    const std::byte *poolLocation = &this->m_arena[id * POOL_SIZE_BYTES];

    // Return initialized pool.
    return reinterpret_cast<const sge::ComponentPool<T>*>(poolLocation);
}

#endif // SGE_COMPONENT_MANAGER_H