#ifndef SGE_SPARSE_PAGE_TABLE_H
#define SGE_SPARSE_PAGE_TABLE_H
#include <memory>
#include <vector>
#include <cstring>
#include <cstdint>

namespace sge {
template<std::unsigned_integral T, size_t PAGE_BITS = 10, typename Allocator = std::allocator<T>>
class SparsePageTable {
public:
    static constexpr size_t ELEMENTS_PER_PAGE =  1ULL << PAGE_BITS;
    static constexpr size_t PAGE_MASK = ELEMENTS_PER_PAGE - 1;
    static constexpr T TOMBSTONE = std::numeric_limits<T>::max();
    using value_type    = T;
    using AllocTraits   = std::allocator_traits<Allocator>;
    using PageAlloc     = typename AllocTraits::template rebind_alloc<T>;
    using PageTraits    = std::allocator_traits<PageAlloc>;
    using PagePtrAlloc  = typename AllocTraits::template rebind_alloc<T*>;

    static_assert(
        AllocTraits::propagate_on_container_move_assignment::value ||
        AllocTraits::is_always_equal::value,
        "Error: [SparsePageTable] requires an allocator type that propagates on move or is always equal."
    );
    
private:

    [[no_unique_address]] PageAlloc         m_pageAlloc;
    std::vector<T*, PagePtrAlloc>           m_pages;

    [[nodiscard]] size_t getPageIndex(size_t index) const noexcept;

    [[nodiscard]] size_t getElementOffset(size_t index) const noexcept;

    [[nodiscard]] T* getOrCreatePage(size_t pageIndex);
public:
    explicit SparsePageTable(const Allocator &alloc = Allocator());

    /////////////////////
    // Big 5
    ////////////////////
    ~SparsePageTable();

    SparsePageTable(const SparsePageTable &other) = delete;
    SparsePageTable& operator=(const SparsePageTable &other) = delete;

    SparsePageTable(SparsePageTable&& other) noexcept;
    SparsePageTable& operator=(SparsePageTable&& other) noexcept (
        AllocTraits::propagate_on_container_move_assignment::value ||
        AllocTraits::is_always_equal::value
    );

    /////////////
    // Accessors
    /////////////
    T& get(size_t index);

    /////////////////
    // Mutators
    /////////////////
    void set(size_t index, const T &value);

    void erase(size_t index);

    void clear() noexcept;
    void reserve(size_t newCapacity);
    void resize(size_t newSize); // TODO
    void release();

    ///////////////////
    // Metadata
    //////////////////
    bool contains(size_t index) const noexcept;
}; // class SparsePageTable
} // namespace sge

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline size_t sge::SparsePageTable<T, PAGE_BITS, Allocator>::getElementOffset(size_t index) const noexcept
{
    return index & PAGE_MASK;
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline size_t sge::SparsePageTable<T, PAGE_BITS, Allocator>::getPageIndex(size_t index) const noexcept
{
    return index >> ELEMENTS_PER_PAGE;
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline T* sge::SparsePageTable<T, PAGE_BITS, Allocator>::getOrCreatePage(size_t pageIndex)
{
    if(pageIndex >= this->m_pages.size())
    {
        this->m_pages.resize(pageIndex + 1, nullptr);
    }

    T* page = this->m_pages[pageIndex];

    if (!page)
    {
        page = PageTraits::allocate(this->m_pageAlloc, ELEMENTS_PER_PAGE);

        std::fill(page, page + ELEMENTS_PER_PAGE, TOMBSTONE);
        
        this->m_pages[pageIndex] = page;
    }

    return page;
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline sge::SparsePageTable<T, PAGE_BITS, Allocator>::SparsePageTable(const Allocator &alloc) :
m_elementAlloc(alloc),
m_pages(PagePtrAlloc(alloc)),
m_size(0)
{}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline sge::SparsePageTable<T, PAGE_BITS, Allocator>::~SparsePageTable()
{
    this->release();
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline sge::SparsePageTable<T, PAGE_BITS, Allocator>::SparsePageTable(SparsePageTable&& other) noexcept :
m_pageAlloc(std::move(other.m_pageAlloc)),
m_pages(std::move(other.m_pages))
{}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline sge::SparsePageTable<T, PAGE_BITS, Allocator>& sge::SparsePageTable<T, PAGE_BITS, Allocator>::operator=(SparsePageTable &&other)
noexcept (
    AllocTraits::propagate_on_container_move_assignment::value ||
    AllocTraits::is_always_equal::value
)
{
    if (this == &other) return *this;

    this->release();

    constexpr bool propagate = AllocTraits::propagate_container_move_assignment::value;

    if constexpr (propagate)
    {
        this->m_pageAlloc = std::move(other.m_pageAlloc);
    }
    else
    {
        assert(this->m_pageAlloc == other.m_pageAlloc && "[SparsePageTable]::operator=(SparsePageTable&&) | Error. Allocators must match if propagation is disabled.");
    }

    this->m_pages = std::move(other.m_pages);

    return *this;
}

template <std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline T& sge::SparsePageTable<T, PAGE_BITS, Allocator>::get(size_t index)
{
    size_t pageIndex = this->getPageIndex(index);
    size_t offset = this->getElementOffset(index);
    
    assert(pageIndex < this->m_pages.size() && "[SparsePageTable]::get(size_t index) | Error. index out of bounds.");

    PageType *page = this->getOrCreatePage(pageIndex);

    assert(page != nullptr && "[SparsePageTable]::get(size_t index) | Error. Attempt to get from unallocated page.");

    return *page[offset];
}

template <std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline void sge::SparsePageTable<T, PAGE_BITS, Allocator>::set(size_t index, const T &value)
{
    size_t pageIndex = this->getPageIndex(index);
    size_t offset = this->getElementOffset(index);
    
    T* page = this->getOrCreatePage(pageIndex);

    page[offset] = value;
}

template <std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline void sge::SparsePageTable<T, PAGE_BITS, Allocator>::erase(size_t index)
{
    size_t pageIndex = this->getPageIndex(index);

    assert(pageIndex < this->m_pages.size() && "[SparsePageTable]::erase(size_t index) | Error. Attempt to erase outside of bounds.");

    T* page = this->m_pages[pageIndex];

    assert(page != nullptr && "[SparsePageTable]::erase(size_t index) | Error. Attempt to erase slot in uninitialized page.");

    size_t offset = this->getElementOffset(index);

    page[offset] = TOMBSTONE;
}


template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline void sge::SparsePageTable<T, PAGE_BITS, Allocator>::clear() noexcept
{
    for (T* page : this->m_pages)
    {
        if (page)
        {
            std::fill(page, page + ELEMENTS_PER_PAGE, TOMBSTONE);
        }
    }
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline void sge::SparsePageTable<T, PAGE_BITS, Allocator>::reserve(size_t newCapacity)
{
    size_t requiredPages = this->getPageIndex(newCapacity) + 1;

    this->m_pages.reserve(requiredPages);
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline void sge::SparsePageTable<T, PAGE_BITS, Allocator>::resize(size_t newSize)
{
    size_t requiredPages = this->getPageIndex(newSize) + 1;

    if (requiredPages > this->m_pages.size())
    {
        this->m_pages.resize(requiredPages, nullptr);
    }
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline void sge::SparsePageTable<T, PAGE_BITS, Allocator>::release()
{
    for (T* page : this->m_pages)
    {
        if (page)
        {
            PageTraits::deallocate(this->m_pageAlloc, page, ELEMENTS_PER_PAGE);
        }
    }

    this->m_pages.clear();
}

template<std::unsigned_integral T, size_t PAGE_BITS, typename Allocator>
inline bool sge::SparsePageTable<T, PAGE_BITS, Allocator>::contains(size_t index) const noexcept
{
    size_t pageIndex = this->getPageIndex(index);

    T* page = this->m_pages[pageIndex];

    if (!page) return false;

    size_t offset = this->getElementOffset(index);

    return page[offset] != TOMBSTONE;
}

#endif // SGE_SPARSE_PAGE_TABLE_H