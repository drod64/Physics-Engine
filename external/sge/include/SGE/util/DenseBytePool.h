#ifndef SGE_DENSE_BYTE_POOL
#define SGE_DENSE_BYTE_POOL
#include <SGE/util/ByteStream.h>

namespace sge {
/**
 * Helpful utility class to write object data of the same type to one uniform pool (stored as raw bytes).
 * Unlike ByteStream, this class supports random access through indexing (0, 1, 2 ...N).
 * Since this class is not templated, it has no idea of the underlying type it is storing.
 * Upon construction you must pass sizeof(T), where T represents your desired object type to store.
 * 
 * NOTE: Objects are expected to be of the same type.
 * Passing in object data differing from what the pool expects to hold will cause undefined behavior.
 */
class DenseBytePool {
private:
    ByteStream      m_byteStream;
    const size_t    m_OBJECT_SIZE;

public:
    /**
     * Explicit parameterized constructor.
     * @param objectSize the size of the data/object (use sizeof(T))
     * @param initialCount the initial count of objects to allocate memory for
     */
    explicit DenseBytePool(size_t objectSize, size_t initialCount);

    /**
     * Write object data at an index.
     * 
     * NOTE: Object's are expected to be of the same type.
     * @param index the index position to write the data
     * @param srcData a pointer to the data to write into the pool
     */
    void writeData(size_t index, const void *srcData);

    /**
     * Read data at an index.
     * 
     * NOTE: This simply copies the bytes to your desired address.
     * Any updates to the copy will NOT reflect in this DenseBytePool.
     * @param index the index position to read the data from
     * @param outData the external pointer address to pass the data to 
     */
    void readData(size_t index, void *outData);

    /**
     * Copies data from one index to another destination index.
     * @param srcIndex the source index that will be copied
     * @param destIndex the destination index that will house the copied bytes
     */
    void copyData(size_t srcIndex, size_t destIndex);

    /**
     * @return a non-modifiable type-erased pointer to the raw data in bytes
     */
    const void* getRawData() const;

    /**
     * @return a modifiable type-erased pointer to the raw data in bytes
     * 
     * TIP: use T *object = reinterpret_cast<T*>(static_cast<unsigned char*>(baseBuffer) + offset)
     * to get the object pointer to modify it. This WILL reflect in the DenseBytePool.
     */
    void* getRawData();

    /**
     * @return the capacity (in bytes) of the DenseBytePool
     */
    size_t capacity() const;

    /**
     * Resets the read/write pointers.
     * Use this when your old data has no use and you want to start writing from the beginning.
     */
    void clear();
};
} // namespace sge

// Implementation

inline sge::DenseBytePool::DenseBytePool(size_t objectSize, size_t initialCount) :
m_byteStream(objectSize * initialCount),
m_OBJECT_SIZE(objectSize)
{}

inline void sge::DenseBytePool::writeData(size_t index, const void *srcData)
{
    this->m_byteStream.setWritePosition(index * this->m_OBJECT_SIZE);
    this->m_byteStream.writeRawBytes(srcData, this->m_OBJECT_SIZE);
}

inline void sge::DenseBytePool::readData(size_t index, void *outData)
{
    this->m_byteStream.setReadPosition(index * this->m_OBJECT_SIZE);
    this->m_byteStream.readRawBytes(outData, this->m_OBJECT_SIZE);
}

inline void sge::DenseBytePool::copyData(size_t srcIndex, size_t destIndex)
{
    if (srcIndex == destIndex) return;

    size_t srcOffset = srcIndex * this->m_OBJECT_SIZE;
    size_t destOffset = destIndex * this->m_OBJECT_SIZE;

    void *baseBuffer = this->m_byteStream.data();

    std::memmove(
        static_cast<unsigned char*>(baseBuffer) + destOffset,
        static_cast<unsigned char*>(baseBuffer) + srcOffset,
        this->m_OBJECT_SIZE
    );
}

inline const void* sge::DenseBytePool::getRawData() const
{
    return this->m_byteStream.data();
}

inline void* sge::DenseBytePool::getRawData() 
{
    return this->m_byteStream.data();
}

inline size_t sge::DenseBytePool::capacity() const
{
    return this->m_byteStream.capacity();
}

inline void sge::DenseBytePool::clear()
{
    this->m_byteStream.clear();
}

#endif // SGE_DENSE_BYTE_POOL