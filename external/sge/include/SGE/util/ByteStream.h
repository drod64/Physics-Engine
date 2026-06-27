#ifndef SGE_MULTI_BYTE_BUFFER_H
#define SGE_MULTI_BYTE_BUFFER_H
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <new> 

// Handle Windows vs. Unix headers for aligned allocations
#if defined(_WIN32)
    #include <malloc.h>
#else
    #include <cstdlib>
#endif

namespace sge {
class ByteStream {
private:
    uint8_t *m_data;
    size_t m_capacity;
    size_t m_writePointer;
    size_t m_readPointer;

    size_t getAlignedPosition(size_t curPosition, size_t alignment) const;

    void reserve(size_t newCapacity);

public:
    explicit ByteStream(size_t capacity);
    
    ~ByteStream();

    ByteStream(const ByteStream &ByteStream) = delete;
    ByteStream& operator= (const ByteStream &ByteStream) = delete;

    ByteStream(ByteStream &&) noexcept;
    ByteStream& operator= (ByteStream &&) noexcept;

    uint8_t* data();

    size_t size() const;

    size_t getReadPosition() const;

    void setReadPosition(size_t position);

    size_t capacity() const;

    void clear();

    template <typename T>
    size_t write(const T &value);

    template <typename T>
    T& getByHandle(size_t handleIndex);

    template <typename T>
    const T& getByHandle(size_t handleIndex) const;

    void writeRawBytes(const uint8_t *srcData, size_t sizeInBytes);

    void readRawBytes(uint8_t *dest, size_t sizeInBytes);

    template <typename T>
    T read();
}; // class ByteStream
} // namespace sge

// Implementation

inline sge::ByteStream::ByteStream(size_t capacity) :
m_capacity(capacity),
m_data(nullptr),
m_writePointer(0),
m_readPointer(0)
{
    if (this->m_capacity < 64);
    {
        this->m_capacity = 64;
    }

    const size_t ALIGNMENT = 64;
    const size_t REMAINDER = this->m_capacity % ALIGNMENT;

    if (REMAINDER != 0)
    {
        this->m_capacity += (ALIGNMENT - REMAINDER); 
    }

    #if defined(_WIN32)
        // Windows Platform Allocation
        this->m_data = static_cast<uint8_t*>(_aligned_malloc(this->m_capacity, ALIGNMENT));
    #else
        // Linux / Mac / Console Allocation
        void* allocatedPtr = nullptr;
        if (posix_memalign(&allocatedPtr, ALIGNMENT, this->m_capacity) != 0)
        {
            throw std::bad_alloc();
        }
        this->m_data = static_cast<uint8_t*>(allocatedPtr);
    #endif

    if (!this->m_data)
    {
        throw std::bad_alloc();
    }
}

inline sge::ByteStream::~ByteStream()
{
    if (this->m_data)
    {
        #if defined(_WIN32)
            _aligned_free(this->m_data);
        #else
            std::free(this->m_data);
        #endif
    }
}

inline size_t sge::ByteStream::getAlignedPosition(size_t curPosition, size_t alignment) const
{
    return (curPosition + (alignment - 1)) & ~(alignment - 1);
}

inline void sge::ByteStream::reserve(size_t newCapacity)
{
    // Never shrink the byte buffer.
    if (newCapacity <= this->m_capacity) return;
    
    // Enforce 64 byte strict alignment.
    const size_t ALIGNMENT = 64;
    const size_t REMAINDER = newCapacity % ALIGNMENT;
    if (REMAINDER != 0)
    {
        newCapacity += (ALIGNMENT - REMAINDER);
    }

    uint8_t *newData = nullptr;

    // Allocate based on OS
    #if defined(_WIN32)
        newData = static_cast<uint8_t*>(_aligned_malloc(newCapacity, ALIGNMENT));
    #else
        void* allocatedPtr = nullptr;
        if (posix_memalign(&allocatedPtr, ALIGNMENT, newCapacity) != 0)
        {
            throw std::bad_alloc();
        }
        newData = static_cast<uint8_t*>(allocatedPtr);
    #endif

    if (!newData)
    {
        throw std::bad_alloc();
    }

    // Copy old data to new address
    if (this->m_data && this->m_writePointer != 0)
    {
        std::memcpy(newData, this->m_data, this->m_writePointer);
    }

    // Free old data.
    if (this->m_data)
    {
        #if defined(_WIN32)
            _aligned_free(this->m_data);
        #else
            std::free(this->m_data);
        #endif
    }

    // Update pointers/trackers.
    this->m_data = newData;
    this->m_capacity = newCapacity;
}

inline sge::ByteStream::ByteStream(sge::ByteStream &&other) noexcept :
m_data(other.m_data),
m_capacity(other.m_capacity),
m_writePointer(other.m_writePointer),
m_readPointer(other.m_readPointer)
{
    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_readPointer = 0;
    other.m_writePointer = 0;
}

inline sge::ByteStream& sge::ByteStream::operator=(ByteStream &&other) noexcept
{
    if (this != &other)
    {
        if (this->m_data)
        {
            #if defined(_WIN32)
                _aligned_free(this->m_data);
            #else
                std::free(this->m_data);
            #endif
        }

        this->m_data = other.m_data;
        this->m_capacity = other.m_capacity;
        this->m_readPointer = other.m_readPointer;
        this->m_writePointer = other.m_writePointer;

        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_readPointer = 0;
        other.m_writePointer = 0;
    }

    return *this;
}

inline uint8_t* sge::ByteStream::data()
{
    return this->m_data;
}

inline size_t sge::ByteStream::size() const
{
    return this->m_writePointer;
}

inline size_t sge::ByteStream::getReadPosition() const
{
    return this->m_readPointer;
}

inline void sge::ByteStream::setReadPosition(size_t position)
{
    if (position > this->m_writePointer)
    {
        throw std::out_of_range("[ByteStream]::setReadPosition: Target position exceeds active stream size boundaries.\n");
    }
    this->m_readPointer = position;
}

inline size_t sge::ByteStream::capacity() const
{
    return this->m_capacity;
}

inline void sge::ByteStream::clear()
{
    this->m_writePointer = 0;
    this->m_readPointer = 0;
}

template <typename T>
inline size_t sge::ByteStream::write(const T &value)
{
    static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable to serialize safely!");

    size_t alignedWrite = this->getAlignedPosition(this->m_writePointer, alignof(T));

    if (alignedWrite + sizeof(T) > this->m_capacity)
    {
        size_t neededCapacity = alignedWrite + sizeof(T);

        this->reserve(std::max(this->m_capacity * 2, neededCapacity));

        alignedWrite = this->getAlignedPosition(this->m_writePointer, alignof(T));
    }

    if (alignedWrite > this->m_writePointer)
    {
        std::memset(this->m_data + this->m_writePointer, 0, alignedWrite - this->m_writePointer);
    }

    this->m_writePointer = alignedWrite;

    size_t dataHandleIndex = this->m_writePointer;

    std::memcpy(this->m_data + this->m_writePointer, &value, sizeof(T));
    this->m_writePointer += sizeof(T);

    return dataHandleIndex;
}

template <typename T>
inline T& sge::ByteStream::getByHandle(size_t handleIndex)
{
    if (handleIndex + sizeof(T) > this->m_writePointer)
    {
        throw std::out_of_range("[ByteStream]::getByHandle: Handle index + payload size overflows written buffer.\n");
    }

    if ((handleIndex & (alignof(T) - 1)) != 0)
    {
        throw std::invalid_argument("[ByteStream]::getByHandle: Handle address is improperly aligned for target type.\n");
    }

    return *reinterpret_cast<T*>(this->m_data + handleIndex);
}

template <typename T>
inline const T& sge::ByteStream::getByHandle(size_t handleIndex) const
{
    if (handleIndex + sizeof(T) > this->m_writePointer)
    {
        throw std::out_of_range("[ByteStream]::getByHandle: Handle index + payload size overflows written buffer.\n");
    }

    if ((handleIndex & (alignof(T) - 1)) != 0)
    {
        throw std::invalid_argument("[ByteStream]::getByHandle: Handle address is improperly aligned for target type.\n");
    }
    return *reinterpret_cast<const T*>(this->m_data + handleIndex);
}

inline void sge::ByteStream::writeRawBytes(const uint8_t *srcData, size_t sizeInBytes)
{
    if (this->m_writePointer + sizeInBytes > this->m_capacity)
    {
        size_t neededCapacity = this->m_writePointer + sizeInBytes;
        this->reserve(std::max(this->m_capacity * 2, neededCapacity));
    }

    std::memcpy(this->m_data + this->m_writePointer, srcData, sizeInBytes);
    this->m_writePointer += sizeInBytes;
}

inline void sge::ByteStream::readRawBytes(uint8_t *dest, size_t sizeInBytes)
{
    if (this->m_readPointer + sizeInBytes > this->m_writePointer)
    {
        throw std::out_of_range("[ByteStream]::readRawBytes: Stream read overflow out-of-bounds.\n");
    }

    std::memcpy(dest, this->m_data + this->m_readPointer, sizeInBytes);
    this->m_readPointer += sizeInBytes;
}

template <typename T>
inline T sge::ByteStream::read()
{
    static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable to deserialize safely.");

    size_t alignedRead = this->getAlignedPosition(this->m_readPointer, alignof(T));

    if (alignedRead + sizeof(T) > this->m_writePointer)
    {
        throw std::out_of_range("[ByteStream]::read: Stream read overflow out-of-bounds.");
    }

    this->m_readPointer = alignedRead;

    T value;

    std::memcpy(&value, this->m_data + this->m_readPointer, sizeof(T));
    this->m_readPointer += sizeof(T);

    return value;
}


#endif // SGE_MULTI_BYTE_BUFFER_H