#ifndef SGE_VIEW_H
#define SGE_VIEW_H
#include <vector>
#include <limits>
#include <array>
#include <SGE/entities/Entity.h>
#include <SGE/components/IComponentPool.h>

namespace sge {
class Registry;

template <typename... Components>
class View {
private:
    Registry &m_registry;

    // Cached component pools.
    std::array<IComponentPool*, sizeof...(Components)> m_cachedPools;

    // Smallest pool to iterate over.
    IComponentPool *m_iterationPool;

public:
    View(Registry &registry);

    struct iterator {
        size_t index;
        View& view;
    
        iterator(size_t index, View &view);

        Entity operator*() const;
        void operator++();
        bool operator!=(const iterator &other) const;

    private:
        void dropInvalid();
    };

    iterator begin();
    iterator end();

    template <typename T>
    T& get(Entity e);
};
} // namespace sge


// Implementation
#include <SGE/core/Registry.h>

template <typename... Components>
inline sge::View<Components...>::View(sge::Registry &registry) :
m_registry(registry),
m_iterationPool(nullptr)
{
    size_t idx = 0;
    // Cache pools
    ((this->m_cachedPools[idx++] = this->m_registry.getPoolInterface<Components>()), ...);

    // Find smallest pool of components to iterate over.
    size_t minSize = std::numeric_limits<size_t>::max();
    for (sge::IComponentPool *pool : this->m_cachedPools)
    {    
        if (pool->size() < minSize)
        {
            minSize = pool->size();
            this->m_iterationPool = pool;
        }
    }

}

template <typename... Components>
inline sge::View<Components...>::iterator::iterator(size_t index, View &view) :
index(index),
view(view)
{}

template <typename... Components>
inline sge::Entity sge::View<Components...>::iterator::operator*() const
{
    return this->view.m_iterationPool->getEntityAt(this->index);
}

template <typename... Components>
inline void sge::View<Components...>::iterator::operator++()
{
    this->index++;
    this->dropInvalid();
}

template <typename... Components>
inline bool sge::View<Components...>::iterator::operator!=(const iterator &other) const
{
    return this->index != other.index;
}

template <typename... Components>
inline void sge::View<Components...>::iterator::dropInvalid()
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
        for (IComponentPool *pool : this->view.m_cachedPools)
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

template <typename... Components>
inline typename sge::View<Components...>::iterator sge::View<Components...>::begin()
{
    return iterator(0, *this);
}

template <typename... Components>
inline typename sge::View<Components...>::iterator sge::View<Components...>::end()
{
    return iterator(this->m_iterationPool ? m_iterationPool->size() : 0, *this);
}

template <typename... Components>
template<typename T>
inline T& sge::View<Components...>::get(Entity e)
{
    return this->m_registry.getComponent<T>(e);
}

#endif // SGE_VIEW_H