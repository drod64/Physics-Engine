#ifndef SGE_DENSE_BYTE_POOL
#define SGE_DENSE_BYTE_POOL
#include <SGE/util/ByteStream.h>

namespace sge {
class DenseBytePool {
private:
    ByteStream      m_byteStream;
    const size_t    m_OBJECT_SIZE;

public:
    explicit DenseBytePool(size_t objectSize, size_t initialCount);

    void writeData(size_t index, const void *srcData);

    void readData(size_t index, void *outData);

    void copyData(size_t srcIndex, size_t destIndex);

    const void* getRawData() const;

    void* getRawData();

    size_t capacity() const;

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