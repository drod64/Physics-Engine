#ifndef SGE_BYTE_STREAM_H
#define SGE_BYTE_STREAM_H
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <SGE/util/ByteSwap.h>

// Handle Windows vs. Unix headers for aligned allocations
#if defined(_WIN32)
    #include <malloc.h>
#else
    #include <cstdlib>
#endif

namespace sge {
/**
 * Very useful utility class designed to make low-level byte writing/reading a little easier.
 * This class is ideal for mixed-type sequential storage.
 * 
 * NOTE: If you use this class as a sequential stream, read in the exact same order that you write.
 * This will prevent any corruption or crashes. 
 */
class ByteStream {
private:
    unsigned char*  m_data = nullptr;
    uint64_t        m_capacity = 0;
    uint64_t        m_writePointer = 0;
    uint64_t        m_readPointer = 0;
    bool            m_hasFailed = false;

    /**
     * Helper function.
     * Move assignment logic.
     */
    void moveFrom(ByteStream &&other) noexcept;

    /**
     * Helper function.
     * Frees allocated memory.
     */
    void release();

public:
    ByteStream() = default;

    /**
     * Explicit parameterized constructor.
     * @param capacity the initial capacity of the ByteStream 
     */
    explicit ByteStream(uint64_t capacity);
    
    /**
     * Destructor.
     */
    ~ByteStream();

    ByteStream(const ByteStream &ByteStream) = delete;
    ByteStream& operator= (const ByteStream &ByteStream) = delete;

    /**
     * Move constructor.
     * @param other the ByteStream to move
     */
    ByteStream(ByteStream&& other) noexcept;

    /**
     * Assignment move operator.
     * @param other the ByteStream to move
     */
    ByteStream& operator= (ByteStream&& other) noexcept;

    /**
     * Allocates enough space to fit the new capacity.
     * @param newCapacity the new capacity limit of the ByteStream
     */
    void reserve(uint64_t newCapacity);

    /**
     * Resets the read/write pointers.
     */
    void clear();

    /**
     * @return the write position of the stream
     */
    uint64_t getWritePosition() const;

    /**
     * @return the read position of the stream
     */
    uint64_t getReadPosition() const;

    /**
     * Sets the write position.
     * @param position the desired position
     */
    void seekWrite(uint64_t position);

    /**
     * Sets the read position.
     * @param position the desired position
     */
    void seekRead(uint64_t position);

    /**
     * Write to the stream.
     * @tparam T the value type
     * @param value a fundamental or enum type value
     */
    template <typename T>
    void write(const T &value);
    
    /**
     * Read from the stream.
     * @tparam T the value type
     * @return a local-copy of the value read
     */
    template <typename T>
    T read();

    /**
     * Write a trivially copyable struct into the stream.
     * @param podStruct the struct to write into the stream
     * @param explicitAlignment bypasses alignof(T) and applies explicit alignment (when greater than 0)
     */
    template <typename T>
    void writeStruct(const T &podStruct, uint64_t explicitAlignment = 0);

    /**
     * Read a trivially copyable struct from the stream.
     * @param podStruct the struct to read from the stream
     * @param explicitAlignment bypasses alignof(T) and applies explicit alignment (when greater than 0)
     */
    template <typename T>
    T readStruct(uint64_t explicitAlignment = 0);

    /**
     * Write a string to the stream.
     * @param str the string to write into the stream
     */
    void writeString(const std::string &str);

    /**
     * Read a string from the buffer.
     * @return the string
     */
    std::string readString();

    /**
     * Write a chunk of data to the stream.
     * @param src the source address of the data
     * @param size the size of the data
     */
    void writeBytes(const void *src, uint64_t size);

    /**
     * Read a chunk of data from the stream.
     * @param dest the destination address to store the data
     * @param size the size of the data
     */
    void readBytes(void *dest, uint64_t size);

    /**
     * Align the write pointer.
     * @param alignment the desired alignment
     */
    void alignWriteTo(uint64_t alignment);

    /**
     * Align the read pointer.
     * @param alignment the desired alignment
     */
    void alignReadTo(uint64_t alignment);

    /**
     * Skips the read pointer ahead.
     * @param size the amount of bytes to skip
     */
    void skipRead(uint64_t size);

    /**
     * @return a type-erased pointer to the start of the byte stream data
     */
    void* data() noexcept;

    /**
     * @return a const type-erased pointer to the start of the byte stream data
     */
    const void* data() const noexcept;

    /**
     * @return the size of the byte stream
     */
    size_t size() const noexcept;

    /**
     * @return the capacity of the byte stream
     */
    size_t capacity() const noexcept;

    /**
     * @return the state of the byte stream
     */
    bool hasFailed() const noexcept;
}; // class ByteStream
} // namespace sge

// Implementation
inline void sge::ByteStream::moveFrom(sge::ByteStream &&other)
{
    this->m_data = other.m_data;
    this->m_capacity = other.m_capacity;
    this->m_hasFailed = other.m_hasFailed;
    this->m_writePointer = other.m_writePointer;
    this->m_readPointer = other.m_readPointer;

    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_writePointer = 0;
    other.m_readPointer = 0;
    other.m_hasFailed = 0;
}

inline void sge::ByteStream::release()
{
    if (this->m_data)
    {
        #if defined(_WIN32)
            _aligned_free(this->m_data);
        #else
            std::free(this->m_data);
        #endif

        this->m_data = nullptr;
    }
}

inline sge::ByteStream::ByteStream(uint64_t capacity)
{
    this->reserve(capacity);
}

inline sge::ByteStream::~ByteStream()
{
    this->release();
}

inline sge::ByteStream::ByteStream(sge::ByteStream &&other) noexcept
{
    this->moveFrom(std::move(other));
}

inline sge::ByteStream& sge::ByteStream::operator=(ByteStream &&other) noexcept
{
    if (this != &other)
    {
        // Free allocated memory.
        this->release();

        // Move.
        this->moveFrom(std::move(other));
    }

    return *this;
}

inline void sge::ByteStream::reserve(uint64_t newCapacity)
{
    // Never shrink the byte buffer.
    if (newCapacity <= this->m_capacity) return;
    
    // Enforce 64 byte strict alignment.
    const uint64_t ALIGNMENT = 64;
    const uint64_t REMAINDER = newCapacity % ALIGNMENT;
    if (REMAINDER != 0)
    {
        newCapacity += (ALIGNMENT - REMAINDER);
    }

    unsigned char *newData = nullptr;

    // Allocate based on OS
    #if defined(_WIN32)
        newData = static_cast<unsigned char*>(_aligned_malloc(newCapacity, ALIGNMENT));
    #else
        void* allocatedPtr = nullptr;
        if (posix_memalign(&allocatedPtr, ALIGNMENT, newCapacity) != 0)
        {
            throw std::bad_alloc();
        }
        newData = static_cast<unsigned char*>(allocatedPtr);
    #endif

    if (!newData)
    {
        throw std::bad_alloc();
    }

    // Copy old data to new address
    if (this->m_data && this->m_writePointer > 0)
    {
        std::memcpy(newData, this->m_data, this->m_writePointer);
    }

    // Free old data.
    if (this->m_data)
    {
        this->release();
    }

    // Update pointers/trackers.
    this->m_data = newData;
    this->m_capacity = newCapacity;
}

inline void sge::ByteStream::clear()
{
    this->m_writePointer = 0;
    this->m_readPointer = 0;
}

inline uint64_t sge::ByteStream::getWritePosition() const
{
    return this->m_writePointer;
}

inline uint64_t sge::ByteStream::getReadPosition() const
{
    return this->m_readPointer;
}

inline void sge::ByteStream::seekWrite(uint64_t position)
{
    if (position > this->m_capacity)
    {
        this->m_hasFailed = true;
        return;
    }

    if (position > this->m_writePointer)
    {
        std::memset(this->m_data + this->m_writePointer, 0, position - this->m_writePointer);
    }

    this->m_writePointer = position;
}

inline void sge::ByteStream::seekRead(uint64_t position)
{
    if (this->m_hasFailed) return;

    if (position > this->m_writePointer)
    {
        this->m_hasFailed = true;
        return;
    }

    this->m_readPointer = position;
}

template <typename T>
inline void sge::ByteStream::write(const T &value)
{
    static_assert(std::is_fundamental_v<T> || std::is_enum_v<T>,
        "Type must a fundamental or enum type to serialize safely!");

    if constexpr (std::is_same_v<T, bool>)
    {
        uint8_t byteBool = value ? 1 : 0;
        this->writeBytes(&byteBool, sizeof(uint8_t));
        return; 
    }

    if constexpr (std::is_floating_point_v<T>)
    {
        static_assert(std::numeric_limits<T>::is_iec559,
            "[ByteStream]::write<T>(const T &value) | Error. Platform must support IEEE 754 standard for floats.");
    }

    T finalValue = value;

    if constexpr (std::endian::native == std::endian::big)
    {
        if constexpr (std::is_integral_v<T> || std::is_enum_v<T>)
        {
            if constexpr (sizeof(T) > 1)
            {
                // Swap byte order.
                finalValue = sge::byteSwap(value);
            }
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            if constexpr (sizeof(T) == 4)
            {
                auto bits = std::bit_cast<uint32_t>(value);
                finalValue = std::bit_cast<T>(sge::byteSwap(bits));
            }
            else if constexpr (sizeof(T) == 8)
            {
                auto bits = std::bit_cast<uint64_t>(value);
                finalValue = std::bit_cast<T>(sge::byteSwap(bits));
            }
        }
    }

    this->writeBytes(&finalValue, sizeof(T));
}

template <typename T>
inline T sge::ByteStream::read()
{
    static_assert(std::is_fundamental_v<T> || std::is_enum_v<T>,
        "Type must a fundamental or enum type to deserialize safely!");
    
    if (this->m_hasFailed)
    {
        return T{};
    }

    if constexpr (std::is_same_v<T, bool>)
    {
        uint8_t byteBool = 0;
        this->readBybtes(&byteBool, sizeof(uint8_t));

        if (this->m_hasFailed) return false;
        return byteBool != 0;
    }

    if constexpr (std::is_floating_point_v<T>)
    {
        static_assert(std::numeric_limits<T>::is_iec559,
            "[ByteStream]::read<T>() | Error. Platform must support IEEE 754 standard for floats.");
    }

    if (this->m_readPointer + sizeof(T) > this->m_writePointer)
    {
        this->m_hasFailed = true;
        return T{};
    }

    T value{};

    this->readbytes(&value, sizeof(T));

    if constexpr (std::endian::native == std::endian::big)
    {
        if constexpr (std::is_integral_v<T> || std::is_enum_v<T>)
        {
            if constexpr (sizeof(T) > 1)
            {
                value = sge::byteSwap(value);
            }
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            if constexpr (sizeof(T) == 4)
            {
                auto bits = std::bit_cast<uint32_t>(value);
                value = std::bit_cast<T>(sge::byteSwap(bits));
            }
            else if constexpr (sizeof(T) == 8)
            {
                auto bits = std::bit_cast<uint64_t>(value);
                value = std::bit_cast<T>(sge::byteSwap(bits));
            }
        }
    }

    return value;
}

template <typename T>
inline void sge::ByteStream::writeStruct(const T &podStruct, uint64_t explicitAlignment)
{
    static_assert(std::is_trivially_copyable_v<T>,
        "[ByteStream]::writeStruct(uint64_t explicitAlignment = 0) | Error. Type must be trivially copyable to serialize safely.");
    
    uint64_t chosenAlignment = (explicitAlignment > 0) ? explicitAlignment : alignof(T);

    this->alignWriteTo(chosenAlignment);

    this->writeBytes(&podStruct, sizeof(T));
}

template <typename T>
inline T sge::ByteStream::readStruct(uint64_t explicitAlignment)
{
    static_assert(std::is_trivially_copyable_v<T>,
        "[ByteStream]::readStruct(uint64_t explicitAlignment = 0) | Error. Type must be trivially copyable to deserialize safely.");
    
    if (this->m_hasFailed) return T{};

    uint64_t chosenAlignment = (explicitAlignment > 0) ? explicitAlignment : alignof(T);

    this->alignReadTo(chosenAlignment);

    if (this->m_readPointer + sizeof(T) > this->m_writePointer)
    {
        this->m_hasFailed = true;
        return T{};
    }

    T result{};

    this->readBytes(&result, sizeof(T));

    return result;
}

inline void sge::ByteStream::writeString(const std::string &str)
{
    uint64_t length = str.size();

    this->write<uint64_t>(length);

    if (length > 0)
    {
        this->writeBytes(str.data(), length);
    }
}

inline std::string sge::ByteStream::readString()
{
    if (this->m_hasFailed) return std::string();

    uint64_t length = this->read<uint64_t>();

    if (this->m_hasFailed) return std::string();

    if (length == 0) return std::string();
    
    if (this->m_readPointer + length > this->m_writePointer)
    {
        this->m_hasFailed = true;
        return std::string();
    }

    std::string str;
    str.resize(static_cast<size_t>(length));

    this->readBytes(str.data(), length);

    return str;
}

inline void sge::ByteStream::writeBytes(const void *src, uint64_t size)
{
    if (!src || size == 0) return;

    uint64_t requiredCapacity = this->m_writePointer + size;
    if (requiredCapacity > this->m_capacity)
    {
        this->reserve(std::max(this->m_capacity * 2, requiredCapacity + 1));
    }

    std::memcpy(this->m_data + this->m_writePointer, src, size);

    this->m_writePointer += size;
}

inline void sge::ByteStream::readBytes(void *dest, uint64_t size)
{
    if (!dest || src == 0) return;

    if (this->m_readPointer + size > this->m_writePointer)
    {
        this->m_hasFailed = true;

        std::memset(dest, 0, size);

        return;
    }

    std::memcpy(dest, this->m_data + this->m_readPointer, size);

    this->m_readPointer += size;
}

inline void sge::ByteStream::alignWriteTo(uint64_t alignment)
{
    const uint64_t remainder = this->m_writePointer % alignment;
    if (remainder != 0)
    {
        uint64_t paddingBytes = alignment - remainder;

        uint8_t zeroPadding[8] = {0};
        this->writeBytes(zeroPadding, paddingBytes);
    }
}

inline void sge::ByteStream::alignReadTo(uint64_t alignment)
{
    const uint64_t remainder = this->m_readPointer % alignment;
    if (remainder != 0)
    {
        uint64_t paddingBytes = alignment - remainder;

        this->seekRead(this->m_readPointer + paddingBytes);
    }
}

inline void sge::ByteStream::skipRead(uint64_t size)
{
    if (this->m_hasFailed) return;

    if (this->m_readPointer + size > this->m_writePointer)
    {
        this->m_hasFailed = true;
        return;
    }

    this->m_readPointer += size;
}

inline void* sge::ByteStream::data() noexcept
{
    return this->m_data;
}

inline const void* sge::ByteStream::data() const noexcept
{
    return this->m_data;
}

inline uint64_t sge::ByteStream::size() const noexcept
{
    return this->m_writePointer;
}

inline uint64_t sge::ByteStream::capacity() const noexcept
{
    return this->m_capacity;
}

inline bool sge::ByteStream::hasFailed() const noexcept
{
    return this->m_hasFailed;
}

#endif // SGE_BYTE_STREAM_H