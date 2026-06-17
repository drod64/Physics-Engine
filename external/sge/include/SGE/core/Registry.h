#ifndef SGE_REGISTRY_H
#define SGE_REGISTRY_H
#include <SGE/entities/EntityManager.h>
#include <SGE/components/ComponentManager.h>

namespace sge {
// Forward declaration of View class
template <typename ...Components>
class View;

class Registry {
private:
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
    bool has(Entity e) const;
    
    // Entity 
    Entity createEntity();

    void destroyEntity(Entity e);

    bool isAlive(Entity e) const;

    template <typename T>
    ComponentPool<T>* getPool();

    template <typename T>
    IComponentPool* getPoolInterface();

    template <typename... Components>
    View<Components...> viewAll();
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
inline bool sge::Registry::has(Entity e) const
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
inline sge::ComponentPool<T>* sge::Registry::getPool()
{
    return this->m_components.getPool<T>();
}

template <typename T>
inline sge::IComponentPool* sge::Registry::getPoolInterface()
{
    return this->m_components.getPoolInterface<T>();
}

template <typename... Components>
inline sge::View<Components...> sge::Registry::viewAll()
{
    return sge::View<Components...>(*this);
}

#endif // SGE_REGISTRY_H