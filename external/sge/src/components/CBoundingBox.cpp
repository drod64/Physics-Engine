#include <SGE/components/CBoundingBox.h>

sge::CBoundingBox::CBoundingBox ()
{
    this->m_size = {0.f, 0.f};
    this->m_halfSize = {0.f, 0.f};
}

sge::CBoundingBox::CBoundingBox(const sge::CBoundingBox &otherBox)
{
    if (this != &otherBox)
    {
        this->m_size = otherBox.m_size;
        this->m_halfSize = otherBox.m_halfSize;
    }
}

sge::CBoundingBox::CBoundingBox(const sm::Vec2 &size)
{
    setSize(size);
}

sm::Vec2 sge::CBoundingBox::size() const
{
    return this->m_size;
}

sm::Vec2 sge::CBoundingBox::halfSize() const
{
    return this->m_halfSize;
}

void sge::CBoundingBox::setSize(const sm::Vec2 &size)
{
    this->m_size = size;
    this->m_halfSize = {size.x / 2.f, size.y / 2.f};
}