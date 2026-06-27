#ifndef SGE_COMPONENT_POOL
#define SGE_COMPONENT_POOL
#include <cassert>
#include <memory>
#include <span>
#include <SGE/util/DenseBytePool.h>
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
    // Check to make sure only trivially copyable (POD) components are allowed.
    static_assert(std::is_trivially_copyable_v<T>, "[ComponentPool]: Error, component type must be trivially copyable");

    /**
     *  Represents inactive entities.
     */
    const size_t m_TOMBSTONE;

    DenseBytePool m_densePool;

    /**
     * Maps each index to its entity owner.
     */
    std::vector<Entity> m_denseToEntity;

    /**
     * The vector containing Entity IDs with a specific sge::Component.
     * Indexing represents using the Entity ID, the value represents
     * the index to use in m_densePool for access to the sge::Component
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
    T& assign(Entity e, U &&component);

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

    std::span<T> getDenseComponents();

    std::vector<Entity>& getDenseToEntities();
    
    const std::span<const T> getDenseComponents() const;

    const std::vector<Entity>& getDenseToEntities() const;
};
} // namespace sge

template <typename T>
inline sge::ComponentPool<T>::ComponentPool(size_t initialSize) :
// Store static functions in base class.
IComponentPool(&sge::ComponentPool<T>::s_removeEntity,
                &sge::ComponentPool<T>::s_clear),
m_TOMBSTONE(0xFFFFFFFF),
m_densePool(sizeof(T), initialSize)
{
    this->m_sparse.resize(initialSize, this->m_TOMBSTONE);
    this->m_denseToEntity.reserve(initialSize);
}

template <typename T>
template <typename U>
inline T& sge::ComponentPool<T>::assign(sge::Entity e, U &&component)
{
    static_assert(std::is_same_v<std::decay_t<U>, T>,
                "[ComponentPool]::assign: Mismatched type! The object passed must match pool template T.");

    assert(!has(e) && "Entity already has this component type!");
    
    size_t e_int = static_cast<size_t>(e);

    // Resize sparse vector if necessary.
    if (e_int >= this->m_sparse.size())
    {
        size_t newSize = this->m_sparse.size() * 2;
        if (newSize <= e_int) newSize = e_int + 1;
        this->m_sparse.resize(newSize, this->m_TOMBSTONE);
    }

    size_t denseIndex = this->m_denseToEntity.size();
    this->m_sparse[e_int] = denseIndex;
    this->m_denseToEntity.push_back(e);

    // Write component data to byte pool.
    this->m_densePool.writeData(denseIndex, std::addressof(component));

    // Find data and return a transient reference.
    void *baseBuffer = this->m_densePool.getRawData();
    size_t offset = denseIndex * sizeof(T);

    return *reinterpret_cast<T*>(static_cast<unsigned char*>(baseBuffer) + offset);
}

template <typename T>
inline void sge::ComponentPool<T>::concreteRemoveEntity(sge::Entity e)
{
    if (!has(e)) return;

    uint32_t e_int = static_cast<uint32_t>(e);
    size_t indexToRemove = this->m_sparse[e_int];
    size_t lastIndex = this->m_denseToEntity.size() - 1;
    
    if (indexToRemove != lastIndex)
    {
        sge::Entity lastEntity = this->m_denseToEntity[lastIndex];
        this->m_densePool.copyData(lastIndex, indexToRemove);
        this->m_denseToEntity[indexToRemove] = lastEntity;
        this->m_sparse[static_cast<size_t>(lastEntity)] = indexToRemove;
    }
    
    // Mark removed entity as dead.
    this->m_sparse[e_int] = this->m_TOMBSTONE;

    // Shrink dense vectors.
    this->m_denseToEntity.pop_back();
}

template <typename T>
inline T& sge::ComponentPool<T>::get(sge::Entity e)
{
    assert(has(e) && "Entity does not have this component type!");
    
    size_t e_int = static_cast<size_t>(e);

    // Find data and return a transient reference.
    void *baseBuffer = this->m_densePool.getRawData();
    size_t offset = this->m_sparse[e_int] * sizeof(T);

    return *reinterpret_cast<T*>(static_cast<unsigned char*>(baseBuffer) + offset);
}

template <typename T>
inline const T& sge::ComponentPool<T>::get(sge::Entity e) const
{
    assert(has(e) && "Entity does not have this component type!");
    
    size_t e_int = static_cast<size_t>(e);

    // Find data and return a transient reference.
    const void *baseBuffer = this->m_densePool.getRawData();
    size_t offset = this->m_sparse[e_int] * sizeof(T);

    return *reinterpret_cast<T*>(static_cast<const unsigned char*>(baseBuffer) + offset);
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
    this->m_densePool.clear();
    this->m_denseToEntity.clear();
    std::fill(this->m_sparse.begin(), this->m_sparse.end(), this->m_TOMBSTONE);
}

template <typename T>
inline sge::Entity sge::ComponentPool<T>::getEntityAt(size_t index) const
{
    return this->m_denseToEntity[index];
}

template <typename T>
inline std::span<T> sge::ComponentPool<T>::getDenseComponents()
{
    T *poolPtr = reinterpret_cast<T*>(this->m_densePool.getRawData());
    return std::span<T>(poolPtr, this->m_denseToEntity.size());
}

template <typename T>
inline std::vector<sge::Entity>& sge::ComponentPool<T>::getDenseToEntities()
{
    return this->m_denseToEntity;
}

template <typename T>
inline const std::span<const T> sge::ComponentPool<T>::getDenseComponents() const
{
    const T *poolPtr = reinterpret_cast<const T*>(this->m_densePool.getRawData());
    return std::span<const T>(poolPtr, this->m_denseToEntity.size());
}

template <typename T>
inline const std::vector<sge::Entity>& sge::ComponentPool<T>::getDenseToEntities() const
{
    return this->m_denseToEntity;
}

#endif // SGE_COMPONENT_POOL