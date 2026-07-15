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

sm::Quaternion sm::Quaternion::conjugate() const
{
    return Quaternion(this->w, -this->x, -this->y, -this->z);
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

sm::Quaternion sm::Quaternion::normalized(const Quaternion &quat)
{
    sm::Quaternion result(quat.w, quat.x, quat.y, quat.z);

    result.normalize();

    return result;
}

sm::Quaternion sm::Quaternion::fromAxisAngle(const sm::Vec3 &axis, sm::real angleRadians)
{
    sm::real halfAngle = angleRadians * static_cast<sm::real>(0.5);
    sm::real sinHalf = real_sin(halfAngle);

    return Quaternion (
        real_cos(halfAngle),
        axis.x * sinHalf,
        axis.y * sinHalf,
        axis.z * sinHalf
    );
}