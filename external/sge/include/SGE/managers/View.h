#ifndef SGE_VIEW_H
#define SGE_VIEW_H
#include <vector>
#include <limits>
#include <tuple>
#include <type_traits>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/ComponentPool.h>

namespace sge {
class Registry;

template <bool isConst, typename... Components>
class ViewImpl {
    private:
    using RegistryRef = std::conditional_t<isConst, const Registry &, Registry &>;
    using ViewRef = std::conditional_t<isConst, const ViewImpl &, ViewImpl &>;

    RegistryRef m_registry;

    // Cached component pools.
    std::tuple<
    std::add_pointer_t<
        std::conditional_t<isConst, const sge::ComponentPool<Components>, sge::ComponentPool<Components>>
    >...
    > m_cachedPools;

    size_t m_iterationPoolIndex;
    size_t m_iterationPoolSize;
    const Entity *m_iterationEntities;

    /**
     * Helper function to get the required component pool ptr.
     * @param registry the active registry to get the pool from
     * @return a pointer to the pool
     */
    template <typename T>
    auto getPoolPtr(RegistryRef registry);

public:
    /**
     * Parameterized constructor.
     * @param registry the active registry to get pool data from
     */
    ViewImpl(RegistryRef registry);

    struct iterator {
        size_t index;
        ViewRef view;
        
        /**
         * Parameterized constructor.
         * @param index the starting index
         * @param view the view reference
         */
        iterator(size_t index, ViewRef view);

        /**
         * Dereferences the iterator.
         * @return a sge::Entity ID
         */
        Entity operator*() const;

        /**
         * Moves the iterator forwards.
         * @return a reference to the iterator
         */
        iterator& operator++();

        /**
         * Checks if two iterators are the same.
         * @return true/false if the iterators match
         */
        bool operator!=(const iterator &other) const;

    private:
        /**
         * Disregards entities that do not match the queried components.
         */
        void dropInvalid();
    };

    /**
     * @return an iterator to the beginning
     */
    iterator begin();

    /**
     * @return an iterator to the end
     */
    iterator end();

    /**
     * @tparam T the component type
     * @param e the sge::Entity ID
     * @return the component that belongs to the entity
     */
    template <typename T>
    std::conditional_t<isConst, const T &, T &> get(Entity e) const;
}; // class View
} // namespace sge


// Implementation
#include <SGE/managers/Registry.h>

template <bool isConst, typename... Components>
template <typename T>
inline auto sge::ViewImpl<isConst, Components...>::getPoolPtr(typename sge::ViewImpl<isConst, Components...>::RegistryRef registry) 
{
    if constexpr (isConst)
    {
        return registry.template getPool<T>();
    }
    else
    {
        return registry.template getOrCreatePool<T>();
    }
}

template <bool isConst, typename... Components>
inline sge::ViewImpl<isConst, Components...>::ViewImpl(typename sge::ViewImpl<isConst, Components...>::RegistryRef registry) :
m_registry(registry),
m_cachedPools{ getPoolPtr<Components>(registry)...},
m_iterationPoolIndex(0),
m_iterationPoolSize(0xFFFFFFFF),
m_iterationEntities(nullptr)
{
    size_t currentIndex = 0;

    auto findSmallest = [&](auto *pool) {
        if (pool && pool->size() < this->m_iterationPoolSize)
        {
            this->m_iterationPoolSize = pool->size();
            this->m_iterationPoolIndex = currentIndex;
            this->m_iterationEntities = pool->getDenseToEntities().data();
        }

        ++currentIndex;
    };

    std::apply([&](auto... pools) { (findSmallest(pools), ...); }, this->m_cachedPools);
}

template <bool isConst, typename... Components>
inline sge::ViewImpl<isConst, Components...>::iterator::iterator(size_t index, typename sge::ViewImpl<isConst, Components...>::ViewRef view) :
index(index),
view(view)
{
    this->dropInvalid();
}

template <bool isConst, typename... Components>
inline sge::Entity sge::ViewImpl<isConst, Components...>::iterator::operator*() const
{
    return this->view.m_iterationEntities[this->index];
}

template <bool isConst, typename... Components>
inline typename sge::ViewImpl<isConst, Components...>::iterator& sge::ViewImpl<isConst, Components...>::iterator::operator++()
{
    this->index++;
    this->dropInvalid();

    return *this;
}

template <bool isConst, typename... Components>
inline bool sge::ViewImpl<isConst, Components...>::iterator::operator!=(const iterator &other) const
{
    return this->index != other.index;
}

template <bool isConst, typename... Components>
inline void sge::ViewImpl<isConst, Components...>::iterator::dropInvalid()
{
    if (this->view.m_iterationPoolSize == 0 || !this->view.m_iterationEntities)
    {
        this->index = 0;
        return;
    }

    while (this->index < this->view.m_iterationPoolSize)
    {
        Entity e = this->view.m_iterationEntities[this->index];

        bool match = std::apply([e](auto... pools) {
            return (pools->has(e) && ...);
        }, this->view.m_cachedPools);

        if (match)
        {
            break;
        }

        this->index++;
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
    return iterator(this->m_iterationPoolSize, *this);
}

template <bool isConst, typename... Components>
template<typename T>
std::conditional_t<isConst, const T &, T &> sge::ViewImpl<isConst, Components...>::get(Entity e) const
{
    auto *pool = std::get<std::conditional_t<isConst, const sge::ComponentPool<T>*,
                                                        sge::ComponentPool<T>*>>(this->m_cachedPools);
    return pool->get(e);
}

#endif // SGE_VIEW_H