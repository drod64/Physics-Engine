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
    // Components
    template <typename T>
    T& addComponent(Entity e, T&& component);

    template <typename T>
    void removeComponent(Entity e);
    
    template <typename T>
    T& getComponent(Entity e);

    template <typename T>
    const T& getComponent(Entity e) const;
    
    template <typename T>
    bool hasComponent(Entity e) const;
    
    // Entity 
    Entity createEntity();

    void destroyEntity(Entity e);

    bool isAlive(Entity e) const;

    template <typename T>
    ComponentPool<T>* getOrCreatePool();

    template <typename T>
    const ComponentPool<T>* getPool() const;

    template <typename... Components>
    View<Components...> viewAll();

    template <typename... Components>
    ConstView<Components...> viewAll() const;

    // Global Context
    template <typename T>
    [[nodiscard]] T& getContext();

    template <typename T>
    [[nodiscard]] const T& getContext() const;

    [[nodiscard]] GlobalContext& getGlobalContext();

    void lockGlobalContext();

    template <typename T>
    bool hasContext() const;
};
} // namspace sge

// Implementation

template <typename T>
inline T& sge::Registry::addComponent(sge::Entity e, T&& component)
{
    return this->m_components.addComponent<T>(e, std::forward<T>(component));
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