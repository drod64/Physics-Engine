#ifndef SGE_COMPONENT_POOL
#define SGE_COMPONENT_POOL
#include <vector>
#include <SGE/entities/Entity.h>
#include <SGE/components/IComponentPool.h>

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

public:
    /**
     * Parameterized Constructor.
     * @param initialSize the initial size of the vector containing Entity IDs
     */
    ComponentPool(size_t initialSize = 1024);

    /**
     * Assigns a component to the specific Entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to assign the component to
     * @param component the component to assign
     * @return a reference to the component
     */
    T& assign(Entity e, T&& component);

    /**
     * NOTE: This is an overriden function.
     * 
     * Removes an entity from the pool.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to remove
     */
    void removeEntity(Entity e) override;

    /**
     * Retrives the component from the specific entity.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to get the component from
     * @return a reference to the component
     */
    T& get(Entity e);

    /**
     * Checks if an entity has a specific component.
     * @tparam T the component type (sge::CTransform, sge::CRigidBody, etc...)
     * @param e the entity to check
     * @return true if the entity has the component, false otherwise
     */
    bool has(Entity e) const override;

    size_t size() const override;

    Entity getEntityAt(size_t index) const override;

    std::vector<T>& getDenseComponents();
};
} // namespace sge

template <typename T>
inline sge::ComponentPool<T>::ComponentPool(size_t initialSize) :
m_TOMBSTONE(0xFFFFFFFF)
{
    this->m_sparse.resize(initialSize, this->m_TOMBSTONE);
}

template<typename T>
inline T& sge::ComponentPool<T>::assign(sge::Entity e, T&& component)
{
    assert(!has(e) && "Entity already has this component type!");
    
    size_t e_int = static_cast<size_t>(e);

    if (e_int >= this->m_sparse.size())
    {
        this->m_sparse.resize(static_cast<size_t>(e_int) + 1, this->m_TOMBSTONE);
    }

    this->m_sparse.at(e_int) = this->m_dense.size();
    this->m_dense.push_back(std::move(component));
    this->m_denseToEntity.push_back(e);

    return this->m_dense.back();
}

template <typename T>
inline void sge::ComponentPool<T>::removeEntity(sge::Entity e)
{
    if (!has(e)) return;

    uint32_t e_int = static_cast<uint32_t>(e);

    size_t indexToRemove = this->m_sparse.at(e_int);
    size_t lastIndex = this->m_dense.size() - 1;
    sge::Entity lastEntity = this->m_denseToEntity.at(lastIndex);

    // Move last entity's component and ID to removed index.
    this->m_dense.at(indexToRemove) = std::move(this->m_dense.at(lastIndex));
    this->m_denseToEntity.at(indexToRemove) = lastEntity;

    // Update last entity to point to it's correct component.
    this->m_sparse.at((uint32_t)lastEntity) = indexToRemove;
    // Mark removed entity as dead.
    this->m_sparse.at(e_int) = this->m_TOMBSTONE;

    // Shrink dense vectors.
    this->m_dense.pop_back();
    this->m_denseToEntity.pop_back();
}

template <typename T>
inline T& sge::ComponentPool<T>::get(sge::Entity e)
{
    assert(has(e) && "Entity does not have this component type!");
    
    return this->m_dense.at(this->m_sparse.at((uint32_t)e));
}

template <typename T>
inline bool sge::ComponentPool<T>::has(sge::Entity e) const
{
    uint32_t e_int = static_cast<uint32_t>(e);

    if (e_int >= this->m_sparse.size()) return false;

    return this->m_sparse.at(e_int) != this->m_TOMBSTONE;
}

template <typename T>
inline size_t sge::ComponentPool<T>::size() const
{
    return this->m_denseToEntity.size();
}

template <typename T>
inline sge::Entity sge::ComponentPool<T>::getEntityAt(size_t index) const
{
    return this->m_denseToEntity.at(index);
}

template <typename T>
inline std::vector<T>& sge::ComponentPool<T>::getDenseComponents()
{
    return this->m_dense;
}

#endif // SGE_COMPONENT_POOL