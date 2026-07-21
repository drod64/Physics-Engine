#ifndef SGE_CONSTRAINT_POOL_H
#define SGE_CONSTRAINT_POOL_H
#include <vector>
#include <span>
#include <queue>
#include <SGE/util/DenseBytePool.h>
#include <SGE/physics/constraints/Constraint.h>
#include <SGE/physics/constraints/IConstraintPool.h>


namespace sge {
template <typename T>
class ConstraintPool : public IConstraintPool {
private:
    static_assert(std::is_trivially_copyable_v<T>, "[ConstraintPool]: Error, constraint type must be trivially copyable.");

    static void s_remove(IConstraintPool *base, Constraint c)
    {
        static_cast<ConstraintPool<T>*>(base)->removeConstraint(c);
    }

    static void s_clear(IConstraintPool *base)
    {
        static_cast<ConstraintPool<T>*>(base)->clear();
    }

    const size_t m_TOMBSTONE;
    DenseBytePool m_densePool;
    std::vector<Constraint> m_denseToConstraint;
    std::vector<size_t> m_sparse;

public:
    /**
     * Parameterized constructor.
     * @param initialSize the initial size of the pool on start-up
     */
    ConstraintPool(size_t initialSize = 1024);

    ~ConstraintPool() = default;

    /**
     * Adds a constraint to the pool.
     * @tparam U lvalue/rvalue of constraint type
     * @param c the sge::Constraint ID handle to assign the object to
     * @param constraint the constraint object data
     */
    template <typename U>
    T& addConstraint(Constraint c, U &&constraint);

    /**
     * Removes the constraint from the pool.
     * @param c the sge::Constraint ID handle
     */
    void removeConstraint(Constraint c);

    /**
     * @param c the sge::Constraint ID handle
     * @return a transient reference to the constraint object.
     */
    T& getConstraint(Constraint c);

    /**
     * @param c the sge::Constraint ID handle
     * @return a const transient reference to the constraint object.
     */
    const T& getConstraint(Constraint c) const;

    /**
     * @return the size of the pool
     */
    size_t size() const;

    /**
     * Clears the pool.
     */
    void clear();

    /**
     * Checks if a sge::Constraint ID handle belongs in this pool.
     * @param c the sge::Constraint ID handle
     * @return true/false whether the sge::Constraint has data in this pool
     */
    bool has(Constraint c) const;

    /**
     * @param denseIndex the dense index
     * @return the sge::Constraint ID handle mapped to the dense index
     */
    Constraint getConstraintAt(size_t denseIndex) const;

    /**
     * @return a list of all the constraint objects
     */
    std::span<T> getDenseConstraints();

    /**
     * @return a const list of all the constraint objects
     */
    std::span<const T> getDenseConstraints() const;

    /**
     * @return the list mapping dense indexes to sge::Constraint ID handles
     */
    const std::vector<Constraint>& getDenseToConstraints() const;
}; // class ConstraintPool
} // namespace sge

// Implementation

template <typename T>
inline sge::ConstraintPool<T>::ConstraintPool(size_t initialSize) :
IConstraintPool(&sge::ConstraintPool<T>::s_remove,
                &sge::ConstraintPool<T>::s_clear),
m_TOMBSTONE(0xFFFFFFFF),
m_densePool(sizeof(T), initialSize)
{
    this->m_sparse.resize(initialSize, this->m_TOMBSTONE);
    this->m_denseToConstraint.reserve(initialSize);
}


template <typename T>
template <typename U>
inline T& sge::ConstraintPool<T>::addConstraint(sge::Constraint c, U &&constraint)
{
    using CleanType = std::remove_cvref_t<U>;

    static_assert(std::is_same_v<CleanType, T>,
    "[ConstraintPool]::addConstraint: Mismatched type! The object passed must match pool template T.");

    assert(!this->has(c) && "Error. Cannot add a constraint to an already active handle\n");
    size_t c_int = static_cast<size_t>(c);

    // Resize if necessary.
    if (c_int >= this->m_sparse.size())
    {
        size_t newSize = this->m_sparse.size() * 2;
        if (newSize <= c_int) newSize = c_int + 1;
        this->m_sparse.resize(newSize, this->m_TOMBSTONE);
    }

    // Update sparse mapping.
    size_t denseIndex = this->m_denseToConstraint.size();
    this->m_sparse[c_int] = denseIndex;
    this->m_denseToConstraint.push_back(c);

    CleanType materialized = std::forward<U>(constraint);

    // Write constraint data.
    this->m_densePool.writeData(denseIndex, std::addressof(materialized));

    // Get data...
    T* poolBuffer = reinterpret_cast<T*>(this->m_densePool.getRawData());

    // ...and return a transient reference.
    return poolBuffer[denseIndex];
}

template <typename T>
inline void sge::ConstraintPool<T>::removeConstraint(sge::Constraint c)
{
    if (!has(c)) return;

    size_t c_int = static_cast<size_t>(c);
    size_t indexToRemove = this->m_sparse[c_int];
    size_t lastIndex = this->m_denseToConstraint.size() - 1;

    if (indexToRemove != lastIndex)
    {
        sge::Constraint lastInstance = this->m_denseToConstraint[lastIndex];
        this->m_densePool.copyData(lastIndex, indexToRemove);
        this->m_denseToConstraint[indexToRemove] = lastInstance;
        this->m_sparse[static_cast<size_t>(lastInstance)] = indexToRemove;
    }

    this->m_sparse[c_int] = this->m_TOMBSTONE;
    this->m_denseToConstraint.pop_back();
}

template <typename T>
T& sge::ConstraintPool<T>::getConstraint(sge::Constraint c)
{
    assert(has(c) && "Constraint handle is not active.");

    size_t c_int = static_cast<size_t>(c);

    T* poolBuffer = reinterpret_cast<T*>(this->m_densePool.getRawData());

    return poolBuffer[this->m_sparse[c_int]];
}

template <typename T>
const T& sge::ConstraintPool<T>::getConstraint(sge::Constraint c) const
{
    assert(has(c) && "Constraint handle is not active.");

    size_t c_int = static_cast<size_t>(c);

    const T* poolBuffer = reinterpret_cast<const T*>(this->m_densePool.getRawData());

    return poolBuffer[this->m_sparse[c_int]];
}

template <typename T>
size_t sge::ConstraintPool<T>::size() const
{
    return this->m_denseToConstraint.size();
}

template <typename T>
void sge::ConstraintPool<T>::clear()
{
    this->m_densePool.clear();
    this->m_denseToConstraint.clear();
    std::fill(this->m_sparse.begin(), this->m_sparse.end(), this->m_TOMBSTONE);
}

template <typename T>
bool sge::ConstraintPool<T>::has(sge::Constraint c) const
{
    size_t c_int = static_cast<size_t>(c);

    if (c_int >= this->m_sparse.size()) return false;

    return this->m_sparse[c_int] != this->m_TOMBSTONE;
}

template <typename T>
sge::Constraint sge::ConstraintPool<T>::getConstraintAt(size_t denseIndex) const
{
    return this->m_denseToConstraint[denseIndex];
}

template <typename T>
std::span<T> sge::ConstraintPool<T>::getDenseConstraints()
{
    T* poolBuffer = reinterpret_cast<T*>(this->m_densePool.getRawData());
    return std::span<T>(poolBuffer, this->m_denseToConstraint.size());
}

template <typename T>
std::span<const T> sge::ConstraintPool<T>::getDenseConstraints() const
{
    const T* poolBuffer = reinterpret_cast<const T*>(this->m_densePool.getRawData());
    return std::span<const T>(poolBuffer, this->m_denseToConstraint.size());
}

template <typename T>
const std::vector<sge::Constraint>& sge::ConstraintPool<T>::getDenseToConstraints() const
{
    return this->m_denseToConstraint;
}

#endif // SGE_CONSTRAINTS_POOL_H