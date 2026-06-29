#ifndef SGE_REGISTRY_H
#define SGE_REGISTRY_H
#include <memory>
#include <cassert>
#include <SGE/managers/EntityManager.h>
#include <SGE/managers/ComponentManager.h>
#include <SGE/core/globalContext/GlobalContext.h>

namespace sge {

template <bool isConst, typename... Components>
class ViewImpl;
template<typename... Components>
using View = sge::ViewImpl<false, Components...>;
template<typename... Components>
using ConstView = sge::ViewImpl<true, Components...>;

class Registry {
private:
    GlobalContext       m_contexts;
    ComponentManager    m_components;
    EntityManager       m_entities;

public:
    /**
     * Adds a component to an existing entity.
     * @tparam T the component type
     * @param e the sge::Entity ID
     * @param component the component object/data
     * @return a transient reference to the newly added component
     */
    template <typename T>
    auto& addComponent(Entity e, T &&component);

    /**
     * Removes a component from an existing entity.
     * @tparam T the component type
     * @param e the sge::Entity ID
     */
    template <typename T>
    void removeComponent(Entity e);
    
    /**
     * Retrives a modifiable component from an existing entity.
     * @tparam T the component type
     * @param e the sge::Entity ID
     * @return a transient reference to the component
     */
    template <typename T>
    T& getComponent(Entity e);

    /**
     * Retrives a non-modifiable component from an existing entity.
     * @tparam T the component type
     * @param e the sge::Entity ID
     * @return a transient reference to the component
     */
    template <typename T>
    const T& getComponent(Entity e) const;
    
    /**
     * Checks if an entity has a component.
     * @tparam T the component type
     * @param e the sge::Entity ID
     * @return true/false if the component has the component
     */
    template <typename T>
    bool hasComponent(Entity e) const;
    
    /**
     * Create a new entity.
     * @return the newly created sge::Entity ID 
     */
    Entity createEntity();

    /**
     * Destroys an entity from the registry (removes all components from the entity as well).
     * @param e the sge::Entity ID to destroy
     */
    void destroyEntity(Entity e);

    /**
     * Checks if an entity is alive/exists.
     * @param e the sge::Entity ID to check
     */
    bool isAlive(Entity e) const;

    /**
     * Retrieves a modifiable pool list of all components of type T.
     * This function will create the pool if it does not exist.
     * @tparam T the component type
     * @return a pointer to the ComponentPool<T>*, it will never return nullptr
     */
    template <typename T>
    ComponentPool<T>* getOrCreatePool();

    /**
     * Retrieves a non-modifiable pool list of all components of type T.
     * @tparam T the component type
     * @return a pointer to the ComponentPool<T>*, nullptr if it does NOT exist
     */
    template <typename T>
    const ComponentPool<T>* getPool() const;

    /**
     * This function returns a light-weight sge::View object of all entities with the queried components.
     * @tparam Components the queried components
     * @return a lightweight sge::View object of queried components
     */
    template <typename... Components>
    View<Components...> viewAll();
/**
     * This function returns a const light-weight sge::View object of all entities with the queried components.
     * @tparam Components the queried components
     * @return a lightweight sge::View object of queried components
     */
    template <typename... Components>
    ConstView<Components...> viewAll() const;

    /**
     * Retrives an already registered global context.
     * @tparam T the context type
     * @return the global context
     */
    template <typename T>
    [[nodiscard]] T& getContext();

    /**
     * Retrives an already registered global context (const).
     * @tparam T the context type
     * @return the global context
     */
    template <typename T>
    [[nodiscard]] const T& getContext() const;

    /**
     * Shortcut to register contexts through the registry.
     */
    template <typename T, typename... Args>
    T& registerContext(Args&&... args);

    /**
     * Retrives the sge::GlobalContext of the registry.
     * @return the sge::GlobalContext
     */
    [[nodiscard]] GlobalContext& getGlobalContext();

    /**
     * Locks the sge::GlobalContext from registering new contexts.
     */
    void lockGlobalContext();

    /**
     * Checks if the registry has a registered context.
     * @return true/false if the registry has the context
     */
    template <typename T>
    bool hasContext() const;
}; // class Registry
} // namspace sge

// Implementation

template <typename T>
inline auto& sge::Registry::addComponent(sge::Entity e, T &&component)
{
    using CleanType = std::decay_t<T>;
    return this->m_components.addComponent<CleanType>(e, std::forward<T>(component));
}

template <typename T>
inline void sge::Registry::removeComponent(sge::Entity e)
{
    this->m_components.removeComponent<T>(e);
}

template <typename T>
inline T& sge::Registry::getComponent(Entity e)
{
    return this->m_components.getComponent<T>(e);
}

template <typename T>
inline const T& sge::Registry::getComponent(Entity e) const
{
    return this->m_components.getComponent<T>(e);
}

template <typename T>
inline bool sge::Registry::hasComponent(Entity e) const
{
    return this->m_components.has<T>(e);
}

inline sge::Entity sge::Registry::createEntity()
{
    return this->m_entities.createEntity();
}

inline void sge::Registry::destroyEntity(sge::Entity e)
{
    this->m_components.entityDestroyed(e);
    this->m_entities.destroyEntity(e);
}

inline bool sge::Registry::isAlive(sge::Entity e) const
{
    return this->m_entities.isAlive(e);
}

template <typename T>
inline sge::ComponentPool<T>* sge::Registry::getOrCreatePool()
{
    return this->m_components.getOrCreatePool<T>();
}

template <typename T>
inline const sge::ComponentPool<T>* sge::Registry::getPool() const
{
    return this->m_components.getPool<T>();
}

template <typename... Components>
inline sge::View<Components...> sge::Registry::viewAll()
{
    return sge::View<Components...>(*this);
}

template <typename... Components>
inline sge::ConstView<Components...> sge::Registry::viewAll() const
{
    return sge::ConstView<Components...>(*this);
}

template <typename T>
inline T& sge::Registry::getContext()
{
    return this->m_contexts.get<T>();
}

template <typename T>
inline const T& sge::Registry::getContext() const
{
    return this->m_contexts.get<T>();
}

template <typename T, typename... Args>
inline T& sge::Registry::registerContext(Args&&... args)
{
    return this->m_contexts.registerContext<T>(std::forward<Args>(args)...);
}

template <typename T>
inline bool sge::Registry::hasContext() const
{
    return this->m_contexts.has<T>();
}

inline sge::GlobalContext& sge::Registry::getGlobalContext()
{
    return this->m_contexts;
}

inline void sge::Registry::lockGlobalContext()
{
    this->m_contexts.lockInitialization();
}

#endif // SGE_REGISTRY_H