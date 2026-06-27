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

    void writeData(size_t index, const uint8_t *srcData);

    void readData(size_t index, uint8_t *outData);

    void copyData(size_t srcIndex, size_t destIndex);

    const uint8_t* getRawData() const;

    size_t capacity() const;
};
} // namespace sge

// Implementation

sge::DenseBytePool::DenseBytePool(size_t objectSize, size_t initialCount) :
m_byteStream(objectSize * initialCount),
m_OBJECT_SIZE(objectSize)
{}

void sge::DenseBytePool::writeData(size_t index, const uint8_t *srcData)
{
    this->m_byteStream.setWritePosition(index * this->m_OBJECT_SIZE);
    this->m_byteStream.writeRawBytes(srcData, this->m_OBJECT_SIZE);
}

void sge::DenseBytePool::readData(size_t index, uint8_t *outData)
{
    this->m_byteStream.setReadPosition(index * this->m_OBJECT_SIZE);
    this->m_byteStream.readRawBytes(outData, this->m_OBJECT_SIZE);
}

void sge::DenseBytePool::copyData(size_t srcIndex, size_t destIndex)
{
    if (srcIndex == destIndex) return;

    size_t srcOffset = srcIndex * this->m_OBJECT_SIZE;
    size_t destOffset = destIndex * this->m_OBJECT_SIZE;

    uint8_t *baseBuffer = this->m_byteStream.data();

    std::memmove(
        baseBuffer + destOffset,
        baseBuffer + srcOffset,
        this->m_OBJECT_SIZE
    );
}

const uint8_t* sge::DenseBytePool::getRawData() const
{
    return this->m_byteStream.data();
}

size_t sge::DenseBytePool::capacity() const
{
    return this->m_byteStream.capacity();
}

#endif // SGE_DENSE_BYTE_POOL