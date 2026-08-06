#ifndef SGE_PAGE_H
#define SGE_PAGE_H
#include <memory>
#include <vector>
#include <cstring>
#include <bitset>
#include <stdexcept>

namespace sge {
template <typename T, size_t PAGE_SIZE, typename Allocator>
class Page {
private:
    using AllocTraits = std::allocator_traits<Allocator>;
    static constexpr OBJECT_SIZE = sizeof(T);

    T* m_data = nullptr;
    size_t m_count = 0;
    std::bitset<PAGE_SIZE> m_initialized;
    [[no_unique_address]] Allocator m_alloc;

    /**
     * Helper move function that is called in rare instances when allocator types don't match.
     * @param other the other Page
     */
    void moveAssignmentElements(Page&& other);

public:
    Page() = default;
    
    /**
     * Destructor. Clears all non-trivial elements and deallocates the Page.
     */
    ~Page();

    Page(const Page &) = delete;
    Page& operator=(const Page&) = delete;

    /**
     * Move constructor.
     * @param other the other Page
     */
    Page(Page&& other) noexcept;

    /**
     * Move assignment.
     * @param other the other Page
     * 
     * @return this
     */
    Page& operator=(Page&& other)
        noexcept (
            AllocTraits::propagate_on_container_move_assignment::value ||
            AllocTraits::is_always_equal::value
        );
    
    /**
     * Allocates a chunk of memory equal to the template argument: PAGE_SIZE.
     */
    void allocate();

    /**
     * Deallocates the chunk of memory from the Page.
     */
    void deallocate();
    
    /**
     * Clears the Page. Destroys all non-trivial types.
     */
    void clear();

    /**
     * Destroys elements from lastValidOffset + 1 - PAGE_SIZE.
     * @param lastValidOffset the last valid offset position
     */
    void truncateElements(size_t lastValidOffset) noexcept;

    /**
     * Removes an element from the Page.
     * @param offset the offset position to erase
     */
    void erase(size_t offset);
    
    /**
     * Sets an element at the desired offset.
     * @param offset the offset position
     * @param value the value
     */
    void setElement(size_t offset, const T &value);

    /**
     * Sets an element at the desired offset.
     * @param offset the offset position
     * @param value the std::move(value)
     */
    void setElement(size_t offset, T&& value);

    /**
     * Emplaces an element at the desired offset using constructor arguments.
     * @tparam Args the constructor arguments of T
     * @param offset the offset position
     * @param args the contructor arugments
     */
    template <typename... Args>
    void emplaceElement(size_t offset, Args&&... args);
    
    /**
     * Retrieves an element from the Page.
     * @param offset the offset position
     * 
     * @return a pointer to the stored element (nullptr if it is unitialized)
     */
    T* getElement(size_t offset);

    /**
     * Retrieves an element from the Page.
     * @param offset the offset position
     * 
     * @return a const pointer to the stored element (nullptr if it is unitialized)
     */
    const T* getElement(size_t offset) const;

    /**
     * @return the current element count of the Page
     */
    size_t count() const noexcept;

    /**
     * Checks if an element is initialized and therefore contained at an offset.
     * @param offset the offset position to check
     * 
     * @return true if the element is initialized
     */
    bool contains(size_t offset) const noexcept;
}; // class Page
} // namespace sge

// Implementation
template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::moveAssignmentElements(Page&& other)
{
    T* newData = AllocTraits::allocate(this->m_alloc, PAGE_SIZE * OBJECT_SIZE);

    size_t constructedCount = 0;

    try
    {
        for (size_t i = 0; i < PAGE_SIZE; ++i)
        {
            if (other.m_initialized.test(i))
            {
                AllocTraits::construct(this->m_alloc, newData[i], std::move(other.m_data[i]));
                ++constructedCount;
            }
        }
    }
    catch(...)
    {
        for (size_t i = 0; i < PAGE_SIZE; ++i)
        {
            if (other.m_initialized.test(i) && constructedCount > 0)
            {
                AllocTraits::destroy(this->m_alloc, newData[i]);
                --constructedCount;
            }
        }

        AllocTraits::deallocate(this->m_alloc, newData, PAGE_SIZE);
        throw;
    }

    this->deallocate();

    this->m_data = newData;
    this->m_initialized = std::move(other.m_initialized);
    this->m_count = other.m_count;

    other.deallocate();
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline sge::Page<T, PAGE_SIZE, Allocator>::~Page()
{
    this->deallocate();
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline sge::Page<T, PAGE_SIZE, Allocator>::Page(Page&& other) noexcept :
m_data(other.m_data),
m_count(other.m_count),
m_initialized(std::move(other.m_initialized)),
m_alloc(std::move(other.m_alloc))
{
    other.m_data = nullptr;
    other.m_count = 0;
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline sge::Page<T, PAGE_SIZE, Allocator>& sge::Page<T, PAGE_SIZE, Allocator>::operator=(Page&& other)
    noexcept (
        AllocTraits::propagate_on_container_move_assignment::value ||
        AllocTraits::is_always_equal::value
    )
{
    if (this == &other) return *this;

    constexpr bool propagate = AllocTraits::propagate_on_container_move_assignment::value;

    if constexpr (propagate)
    {
        this->deallocate();

        this->m_alloc = std::move(other.m_alloc);
        this->m_initialized = std::move(other.m_initialized);
        this->m_data = other.m_data;
        this->m_count = other.m_count;

        other.m_data = nullptr;
        other.m_count = 0;
    }
    else
    {
        if (this->m_alloc == other.m_alloc)
        {
            this->deallocate();
        
            this->m_initialized = std::move(other.m_initialized);
            this->m_data = other.m_data;
            this->m_count = other.m_count;
        
            other.m_data = nullptr;
            other.m_count = 0;
        }
        else
        {
            this->moveAssignmentElements(std::move(other));
        }
    }

    return *this;
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::allocate()
{
    // Prevent allocations if data already has allocated memory.
    if (this->m_data) return;

    // Allocate fresh memory.
    this->m_data = AllocTraits::allocate(this->m_alloc, PAGE_SIZE * OBJECT_SIZE);
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::deallocate()
{
    // Return if there's nothing to deallocate.
    if (!this->m_data) return;

    // Destroy T objects.
    this->clear();

    // Deallocate paged memory.
    AllocTraits::deallocate(this->m_alloc, this->m_data, PAGE_SIZE);

    // Reset data to nullptr.
    this->m_data = nullptr;
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::clear()
{
    // Return if there's nothing to clear.
    if (!this->m_data) return;

    // Call non-trivial destructors.
    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        for (size_t i = 0; i < PAGE_SIZE; ++i)
        {
            // Only destroy initialized offsets.
            if (this->m_initialized.test(i))
            {
                AllocTraits::destroy(this->m_alloc, this->m_data[i]);
            }
        }
    }

    // Reset trackers.
    this->m_count = 0;
    this->m_initialized.reset();
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::truncateElements(size_t lastValidOffset) noexcept
{
    // Return if there's nothing to truncate.
    if (!this->m_data) return;

    // Index check.
    if (lastValidOffset >= PAGE_SIZE) return;

    for (size_t i = lastValidOffset + 1; i < PAGE_SIZE; ++i)
    {
        if (this->m_initialized.test(i))
        {
            // Call non-trivial destructors.
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                AllocTraits::destroy(this->m_alloc, this->m_data[i]);
            }

            --this->m_count;
            this->m_initialized.set(i, false);
        }
    }
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::erase(size_t offset)
{
    // Index check.
    if (offset >= PAGE_SIZE) return;

    // Check if slot is intialized.
    if (this->m_initialized.test(offset))
    {
        // Destroy non-trivial T type...
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            AllocTraits::destroy(this->m_alloc, this->m_data[offset]);
        }

        // Update trackers.
        --this->m_count;
        this->m_initialized.set(offset, false);
    }
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::setElement(size_t offset, const T &value)
{
    // Index check.
    if (offset >= PAGE_SIZE)
    {
        throw std::out_of_range("[Page]::setElement(size_t, const T&, Allocator &) | Error. offset >= PAGE_SIZE");
    }

    // Get target slot.
    T* targetSlot = this->m_data + offset;

    // Construct for the first time...
    if (!this->m_initialized.test(offset))
    {
        AllocTraits::construct(this->m_alloc, targetSlot, value);
        this->m_initialized.set(offset, true);
        ++this->m_count;
    }
    // ...or update exisiting element.
    else
    {
        *targetSlot = value;
    }
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline void sge::Page<T, PAGE_SIZE, Allocator>::setElement(size_t offset, T&& value)
{
    // Index check.
    if (offset >= PAGE_SIZE)
    {
        throw std::out_of_range("[Page]::setElement(size_t, const T&, Allocator &) | Error. offset >= PAGE_SIZE");
    }

    // Get target slot.
    T* targetSlot = this->m_data + offset;

    // Construct for the first time...
    if (!this->m_initialized.test(offset))
    {
        AllocTraits::construct(this->m_alloc, targetSlot, std::move(value));
        this->m_initialized.set(offset, true);
        ++this->m_count;
    }
    // ...or update exisiting element.
    else
    {
        *targetSlot = std::move(value);
    }
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
template <typename... Args>
inline void sge::Page<T, PAGE_SIZE, Allocator>::emplaceElement(size_t offset, Args&&... args)
{
    // Index check.
    if (offset >= PAGE_SIZE)
    {
        throw std::out_of_range("[Page]::setElement(size_t, const T&, Allocator &) | Error. offset >= PAGE_SIZE");
    }

    // Get target slot.
    T* targetSlot = this->m_data + offset;

    // Construct for the first time...
    if (!this->m_initialized.test(offset))
    {
        AllocTraits::construct(this->m_alloc, targetSlot, std::forward<Args>(args));
        this->m_initialized.set(offset, true);
        ++this->m_count;
    }
    // ...or update exisiting element.
    else
    {
        *targetSlot = T(std::forward<Args>(args)...);
    }
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline T* sge::Page<T, PAGE_SIZE, Allocator>::getElement(size_t offset)
{
    if (offset >= PAGE_SIZE)
    {
        throw std::out_of_range("[Page]::getElement(size_t) | Error. offset >= PAGE_SIZE");
    }

    if (!this->m_initialized.test(offset))
    {
        return nullptr;
    }

    return &this->m_data[offset];
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline const T* sge::Page<T, PAGE_SIZE, Allocator>::getElement(size_t offset) const
{
    if (offset >= PAGE_SIZE)
    {
        throw std::out_of_range("[Page]::getElement(size_t) | Error. offset >= PAGE_SIZE");
    }

    if (!this->m_initialized.test(offset))
    {
        return nullptr;
    }

    return &this->m_data[offset];
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline size_t sge::Page<T, PAGE_SIZE, Allocator>::count() const noexcept
{
    return this->m_count;
}

template <typename T, size_t PAGE_SIZE, typename Allocator>
inline bool sge::Page<T, PAGE_SIZE, Allocator>::contains(size_t offset) const noexcept
{
    if (offset >= PAGE_SIZE) return false;

    return this->m_initialized.test(offset);
}

#endif // SGE_PAGE_H