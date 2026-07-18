#include <SM/Quaternion.h>

sm::Quaternion::Quaternion(sm::real x, sm::real y, sm::real z, sm::real w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

sm::Quaternion& sm::Quaternion::operator+= (const sm::Quaternion &other)
{
    (*this) = (*this) + other;

    return *this;
}

sm::Quaternion& sm::Quaternion::operator*=(const sm::Quaternion &other)
{
    (*this) = (*this) * other;

    return *this;
}

sm::Quaternion& sm::Quaternion::operator*=(sm::real scalar)
{
    (*this) = (*this) * scalar;

    return *this;
}

sm::Quaternion& sm::Quaternion::rotateLocal(const sm::Quaternion &rotation)
{
    (*this) = (*this) * rotation;

    return *this;
}

sm::Quaternion& sm::Quaternion::rotateGlobal(const sm::Quaternion &rotation)
{
    (*this) = rotation * (*this);

    return *this;
}

sm::Quaternion& sm::Quaternion::normalize()
{
    sm::real mag = this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w;

    if (mag > static_cast<sm::real>(0))
    {
        sm::real invMag = static_cast<sm::real>(1) / real_sqrt(mag);
        this->x *= invMag;
        this->y *= invMag;
        this->z *= invMag;
        this->w *= invMag;
    }
    else
    {
        this->w = 1;
    }

    return *this;
}

sm::real sm::Quaternion::dot(const sm::Quaternion &lhs, const sm::Quaternion &rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
}

sm::Vec3 sm::Quaternion::rotate(const sm::Vec3 &rotation) const
{
    return (*this) * rotation;
}

sm::Quaternion sm::Quaternion::conjugated() const
{
    return Quaternion(-this->x, -this->y, -this->z, this->w);
}

sm::Quaternion sm::Quaternion::normalized() const
{
    sm::Quaternion result(this->x, this->y, this->z, this->w);

    result.normalize();

    return result;
}