#ifndef SGE_REGISTRY_H
#define SGE_REGISTRY_H
#include <memory>
#include <cassert>
#include <SGE/entities/EntityManager.h>
#include <SGE/components/ComponentManager.h>
#include <SGE/core/resources/ResourceRegistry.h>

namespace sge {

template <bool isConst, typename... Components>
class ViewImpl;
template<typename... Components>
using View = sge::ViewImpl<false, Components...>;
template<typename... Components>
using ConstView = sge::ViewImpl<true, Components...>;

class Registry {
private:
    std::vector<std::unique_ptr<sge::IResourceStorage>> m_resources;
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

    template <typename T>
    IComponentPool* getOrCreatePoolInterface();

    template <typename T>
    const IComponentPool* getPoolInterface() const;

    template <typename... Components>
    View<Components...> viewAll();

    template <typename... Components>
    ConstView<Components...> viewAll() const;

    template <typename T, typename... Args>
    T& getOrCreateResource(Args&&... args);

    template <typename T>
    T& getResource();

    template <typename T>
    const T& getResource() const;

    template <typename T>
    bool hasResource() const;
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

template <typename T>
inline sge::IComponentPool* sge::Registry::getOrCreatePoolInterface()
{
    return this->m_components.getOrCreatePoolInterface<T>();
}

template <typename T>
inline const sge::IComponentPool* sge::Registry::getPoolInterface() const
{
    return this->m_components.getPoolInterface<T>();
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

template <typename T, typename... Args>
T& sge::Registry::getOrCreateResource(Args&&... args)
{
    size_t id = sge::ResourceIDCounter::get<T>();

    if (id >= this->m_resources.size())
    {
        this->m_resources.resize(id + 1);
    }

    // If resource does not exist...
    if (!this->m_resources.at(id))
    {
        // Create concrete storage 
        auto storage = std::make_unique<sge::ConcreteResourceStorage<T>>();

        // Forward constructor arguments.
        storage->value = T(std::forward<Args>(args)...);

        // Assign id index to concrete storage wrapper
        this->m_resources.at(id) = std::move(storage);
    }

    // Get raw pointer to cast it to its type
    auto *concrete = static_cast<sge::ConcreteResourceStorage<T>*>(this->m_resources.at(id).get());

    // Return created
    return concrete->value;
}

template <typename T>
T& sge::Registry::getResource()
{
    assert(this->hasResource<T>() && "Resource does not exist in Registry!");

    size_t id = sge::ResourceIDCounter::get<T>();

    // Get raw pointer to cast it to its type
    auto *concrete = static_cast<sge::ConcreteResourceStorage<T>*>(this->m_resources.at(id).get());

    // Return created
    return concrete->value;
}

template <typename T>
const T& sge::Registry::getResource() const
{
    assert(this->hasResource<T>() && "Const resource does not exist in Registry!");

    size_t id = sge::ResourceIDCounter::get<T>();

    // Get raw pointer to cast it to its type
    auto *concrete = static_cast<sge::ConcreteResourceStorage<T>*>(this->m_resources.at(id).get());

    // Return created
    return concrete->value;
}

template <typename T>
bool sge::Registry::hasResource() const
{
    size_t id = sge::ResourceIDCounter::get<T>();

    return (id < this->m_resources.size() && this->m_resources.at(id));
}

#endif // SGE_REGISTRY_H