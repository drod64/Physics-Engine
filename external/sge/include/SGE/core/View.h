#ifndef SGE_VIEW_H
#define SGE_VIEW_H
#include <vector>
#include <limits>
#include <array>
#include <type_traits>
#include <SGE/entities/Entity.h>
#include <SGE/components/IComponentPool.h>

namespace sge {
class Registry;

template <bool isConst, typename... Components>
class ViewImpl {
private:
    using RegistryRef = std::conditional_t<isConst, const Registry &, Registry &>;
    using PoolPtr = std::conditional_t<isConst, const IComponentPool*, IComponentPool*>;
    using ViewRef = std::conditional_t<isConst, const ViewImpl &, ViewImpl &>;

    RegistryRef m_registry;

    // Cached component pools.
    std::array<PoolPtr, sizeof...(Components)> m_cachedPools;

    // Smallest pool to iterate over.
    PoolPtr m_iterationPool;

    template <typename T>
    PoolPtr getPoolPtr(RegistryRef registry);

public:
    ViewImpl(RegistryRef registry);

    struct iterator {
        size_t index;
        ViewRef view;
    
        iterator(size_t index, ViewRef view);

        Entity operator*() const;
        void operator++();
        bool operator!=(const iterator &other) const;

    private:
        void dropInvalid();
    };

    iterator begin();
    iterator end();

    template <typename T>
    std::conditional_t<isConst, const T &, T &> get(Entity e) const;
};
} // namespace sge


// Implementation
#include <SGE/core/Registry.h>

template <bool isConst, typename... Components>
template <typename T>
inline typename sge::ViewImpl<isConst, Components...>::PoolPtr // Return type
sge::ViewImpl<isConst, Components...>::getPoolPtr(typename sge::ViewImpl<isConst, Components...>::RegistryRef registry)  // Function header
{
    if constexpr (isConst)
    {
        return registry.template getPoolInterface<T>();
    }
    else
    {
        return registry.template getOrCreatePoolInterface<T>();
    }
}

template <bool isConst, typename... Components>
inline sge::ViewImpl<isConst, Components...>::ViewImpl(typename sge::ViewImpl<isConst, Components...>::RegistryRef registry) :
m_registry(registry),
m_iterationPool(nullptr)
{
    size_t idx = 0;
    // Cache pools
    ((this->m_cachedPools[idx++] = this->getPoolPtr<Components>(this->m_registry)), ...);

    // Find smallest pool of components to iterate over.
    size_t minSize = std::numeric_limits<size_t>::max();
    for (auto *pool : this->m_cachedPools)
    {    
        if (pool && pool->size() < minSize)
        {
            minSize = pool->size();
            this->m_iterationPool = pool;
        }
    }
}

template <bool isConst, typename... Components>
inline sge::ViewImpl<isConst, Components...>::iterator::iterator(size_t index, typename sge::ViewImpl<isConst, Components...>::ViewRef view) :
index(index),
view(view)
{}

template <bool isConst, typename... Components>
inline sge::Entity sge::ViewImpl<isConst, Components...>::iterator::operator*() const
{
    return this->view.m_iterationPool->getEntityAt(this->index);
}

template <bool isConst, typename... Components>
inline void sge::ViewImpl<isConst, Components...>::iterator::operator++()
{
    this->index++;
    this->dropInvalid();
}

template <bool isConst, typename... Components>
inline bool sge::ViewImpl<isConst, Components...>::iterator::operator!=(const iterator &other) const
{
    return this->index != other.index;
}

template <bool isConst, typename... Components>
inline void sge::ViewImpl<isConst, Components...>::iterator::dropInvalid()
{
    if (!this->view.m_iterationPool)
    {
        this->index = 0;
        return;
    }

    while (this->index < this->view.m_iterationPool->size())
    {
        Entity e = this->view.m_iterationPool->getEntityAt(this->index);

        // Verify Entity exists in all other cached pools.
        bool match = true;
        for (auto *pool : this->view.m_cachedPools)
        {
            if (pool != this->view.m_iterationPool && (!pool->has(e)))
            {
                match = false;
                break;
            }
        }

        if (match) break;
        ++index;
    }
}

template <bool isConst, typename... Components>
inline typename sge::ViewImpl<isConst, Components...>::iterator sge::ViewImpl<isConst, Components...>::begin()
{
    return iterator(0, *this);
}

template <bool isConst, typename... Components>
inline typename sge::ViewImpl<isConst, Components...>::iterator sge::ViewImpl<isConst, Components...>::end()
{
    return iterator(this->m_iterationPool ? m_iterationPool->size() : 0, *this);
}

template <bool isConst, typename... Components>
template<typename T>
std::conditional_t<isConst, const T &, T &> sge::ViewImpl<isConst, Components...>::get(Entity e) const
{
    return this->m_registry.template getComponent<T>(e);
}

#endif // SGE_VIEW_H