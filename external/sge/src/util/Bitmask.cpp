#include <SGE/util/Bitmask.h>

const int sge::Bitmask::BIT_SIZE = 64;

sge::Bitmask::Bitmask()
{
    this->_mask = 0;
}

sge::Bitmask::Bitmask(const Bitmask& otherBitmask)
{
    this->_mask = otherBitmask._mask;
}

void sge::Bitmask::setBitmask(const Bitmask& otherBitmask)
{
    this->_mask = otherBitmask._mask;
}

void sge::Bitmask::setBitOn(int index)
{
    this->_mask |= (1 << index);
}

bool sge::Bitmask::getBit(int index) const
{
    return (this->_mask & (1 << index));
}

void sge::Bitmask::setBitOff(int index)
{
    this->_mask &= ~(1 << index);
}

void sge::Bitmask::setBit(int index, bool on)
{
    if (on)
    {
        this->setBitOn(index);
    }
    else
    {
        this->setBitOff(index);
    }
}

sge::Bitmask& sge::Bitmask::operator=(const Bitmask &otherBitmask)
= default;

sge::Bitmask sge::Bitmask::operator&(const Bitmask &otherBitmask) const
{
    Bitmask result;
    result._mask = this->_mask & otherBitmask._mask;
    return result;
}

sge::Bitmask sge::Bitmask::operator^(const Bitmask &otherBitmask) const
{
    Bitmask result;
    result._mask = this->_mask ^ otherBitmask._mask;
    return result;
}

sge::Bitmask sge::Bitmask::operator|(const Bitmask &otherBitmask) const
{
    Bitmask result;
    result._mask = this->_mask | otherBitmask._mask;
    return result;
}

sge::Bitmask sge::Bitmask::operator~() const
{
    Bitmask result;
    result._mask = ~this->_mask;
    return result;
}