#include <SM/Quaternion.h>

sm::Quaternion::Quaternion(sm::real w, sm::real x, sm::real y, sm::real z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

sm::Quaternion& sm::Quaternion::operator+= (const sm::Quaternion &other)
{
    this->w += other.w;
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return *this;
}

void sm::Quaternion::normalize()
{
    sm::real mag = real_sqrt(this->w*this->w + this->x*this->x + this->y*this->y + this->z*this->z);

    if (mag > static_cast<sm::real>(0))
    {
        sm::real invMag = static_cast<sm::real>(1) / mag;

        this->w *= invMag;
        this->x *= invMag;
        this->y *= invMag;
        this->z *= invMag;
    }
}

sm::real sm::Quaternion::dot(const sm::Quaternion &lhs, const sm::Quaternion &rhs)
{
    return (lhs.w * rhs.w) + (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

sm::Quaternion sm::Quaternion::conjugated() const
{
    return Quaternion(this->w, -this->x, -this->y, -this->z);
}

sm::Quaternion sm::Quaternion::normalized() const
{
    sm::Quaternion result(this->w, this->x, this->y, this->z);

    result.normalize();

    return result;
}