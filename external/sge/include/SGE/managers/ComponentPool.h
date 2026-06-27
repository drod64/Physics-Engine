#ifndef SGE_COMPONENT_POOL
#define SGE_COMPONENT_POOL
#include <vector>
#include <cassert>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/IComponentPool.h>

namespace sge {
/**
 * Template data structure class that helps with managing a pool of ONE specific sge::Component type.
 * This is a sub-class of sge::IComponentPool
 */
template <typename T>
class ComponentPool : public IComponentPool {
private:
    /**
     *  Represents inactive entities.
     */
    const size_t m_TOMBSTONE;

    /**
     * The vector containing the actual sge::Component objects.
     * use m_sparse to query a specific Entity component.
     * Ex... m_dense[m_sparse[12]] ----> returns the sge::Component of Entity 12.
     */
    std::vector<T> m_dense;

    /**
     * Maps each index to its entity owner.
     */
    std::vector<Entity> m_denseToEntity;

    /**
     * The vector containing Entity IDs with a specific sge::Component.
     * Indexing represents using the Entity ID, the value represents
     * the index to use in m_dense for access to the sge::Component
     */
    std::vector<size_t> m_sparse;

    // Static wrapper functions to store in the IComponent base.
    static void s_removeEntity(IComponentPool *base, Entity e)
    {
        static_cast<ComponentPool<T>*>(base)->concreteRemoveEntity(e);
    }

    static void s_clear(IComponentPool *base)
    {
        static_cast<ComponentPool<T>*>(base)->concreteClear();
    }

public:
    /**
     * Parameterized Constructor.
     * @param initialSize the initial size of the vector containing Entity IDs
     */
    ComponentPool(size_t initialSize = 1024);

    ~ComponentPool() = default;

    /**
     * Assigns a component to the specific Entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to assign the component to
     * @param component the component to assign
     * @return a reference to the component
     */
    template <typename U>
    T& assign(Entity e, U&& component);

    /**
     * NOTE: This is an overriden function.
     * 
     * Removes an entity from the pool.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to remove
     */
    void concreteRemoveEntity(Entity e);

    /**
     * Retrives the component from the specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to get the component from
     * @return a reference to the component
     */
    T& get(Entity e);

    /**
     * Retrives the component from the specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to get the component from
     * @return a reference to the component
     */
    const T& get(Entity e) const;

    /**
     * Checks if an entity has a specific component.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to check
     * @return true if the entity has the component, false otherwise
     */
    bool has(Entity e) const;

    size_t size() const;

    void concreteClear();

    Entity getEntityAt(size_t index) const;

    std::vector<T>& getDenseComponents();

    std::vector<Entity>& getDenseToEntities();
    
    const std::vector<T>& getDenseComponents() const;

    const std::vector<Entity>& getDenseToEntities() const;
};
} // namespace sge

template <typename T>
inline sge::ComponentPool<T>::ComponentPool(size_t initialSize) :
// Store static functions in base class.
IComponentPool(&sge::ComponentPool<T>::s_removeEntity,
                &sge::ComponentPool<T>::s_clear),
m_TOMBSTONE(0xFFFFFFFF)
{
    this->m_sparse.resize(initialSize, this->m_TOMBSTONE);
    this->m_dense.reserve(initialSize);
    this->m_denseToEntity.reserve(initialSize);
}

template <typename T> // raw class type
template <typename U> // reference parameter.
inline T& sge::ComponentPool<T>::assign(sge::Entity e, U&& component)
{
    assert(!has(e) && "Entity already has this component type!");
    
    size_t e_int = static_cast<size_t>(e);

    if (e_int >= this->m_sparse.size())
    {
        size_t newSize = this->m_sparse.size() * 2;
        if (newSize <= e_int) newSize = e_int + 1;
        this->m_sparse.resize(newSize, this->m_TOMBSTONE);
    }

    this->m_sparse[e_int] = this->m_dense.size();
    this->m_dense.push_back(std::move(component));
    this->m_denseToEntity.push_back(e);

    return this->m_dense.back();
}

template <typename T>
inline void sge::ComponentPool<T>::concreteRemoveEntity(sge::Entity e)
{
    if (!has(e)) return;

    uint32_t e_int = static_cast<uint32_t>(e);
    size_t indexToRemove = this->m_sparse[e_int];
    size_t lastIndex = this->m_dense.size() - 1;
    sge::Entity lastEntity = this->m_denseToEntity[lastIndex];

    // Move last entity's component and ID to removed index.
    this->m_dense[indexToRemove] = std::move(this->m_dense[lastIndex]);
    this->m_denseToEntity[indexToRemove] = lastEntity;

    // Update last entity to point to it's correct component.
    this->m_sparse[static_cast<uint32_t>(lastEntity)] = indexToRemove;
    // Mark removed entity as dead.
    this->m_sparse[e_int] = this->m_TOMBSTONE;

    // Shrink dense vectors.
    this->m_dense.pop_back();
    this->m_denseToEntity.pop_back();
}

template <typename T>
inline T& sge::ComponentPool<T>::get(sge::Entity e)
{
    assert(has(e) && "Entity does not have this component type!");
    
    return this->m_dense[this->m_sparse[static_cast<uint32_t>(e)]];
}

template <typename T>
inline const T& sge::ComponentPool<T>::get(sge::Entity e) const
{
    assert(has(e) && "Entity does not have this component type!");
    
    return this->m_dense[this->m_sparse[static_cast<uint32_t>(e)]];
}

template <typename T>
inline bool sge::ComponentPool<T>::has(sge::Entity e) const
{
    uint32_t e_int = static_cast<uint32_t>(e);

    if (e_int >= this->m_sparse.size()) return false;

    return this->m_sparse[e_int] != this->m_TOMBSTONE;
}

template <typename T>
inline size_t sge::ComponentPool<T>::size() const
{
    return this->m_denseToEntity.size();
}

template <typename T>
inline void sge::ComponentPool<T>::concreteClear()
{
    this->m_dense.clear();
    this->m_denseToEntity.clear();
    std::fill(this->m_sparse.begin(), this->m_sparse.end(), this->m_TOMBSTONE);
}

template <typename T>
inline sge::Entity sge::ComponentPool<T>::getEntityAt(size_t index) const
{
    return this->m_denseToEntity[index];
}

template <typename T>
inline std::vector<T>& sge::ComponentPool<T>::getDenseComponents()
{
    return this->m_dense;
}

template <typename T>
inline std::vector<sge::Entity>& sge::ComponentPool<T>::getDenseToEntities()
{
    return this->m_denseToEntity;
}

template <typename T>
inline const std::vector<T>& sge::ComponentPool<T>::getDenseComponents() const
{
    return this->m_dense;
}

template <typename T>
inline const std::vector<sge::Entity>& sge::ComponentPool<T>::getDenseToEntities() const
{
    return this->m_denseToEntity;
}

#endif // SGE_COMPONENT_POOL