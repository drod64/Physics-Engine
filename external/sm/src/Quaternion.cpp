#include <SM/Quaternion.h>

sm::Quaternion::Quaternion(sm::real w, sm::real x, sm::real y, sm::real z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

sm::Quaternion sm::Quaternion::operator* (const sm::Quaternion &other) const
{
    return sm::Quaternion(
        this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z,
        this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y,
        this->w * other.y - this->x * other.z + this->y * other.w + this->z * other.x,
        this->w * other.z + this->x * other.y - this->y * other.x + this->z * other.w
        );
}

sm::Vec3 sm::Quaternion::operator* (const sm::Vec3 &v) const
{
    sm::Vec3 qVec(this->x, this->y, this->z);

    sm::Vec3 cross1 = sm::Vec3::crossProduct(qVec, v);
    sm::Vec3 cross2 = sm::Vec3::crossProduct(qVec, cross1);

    return v + (cross1 * w + cross2) * static_cast<sm::real>(2);
}

sm::Quaternion& sm::Quaternion::operator+= (const sm::Quaternion &other)
{
    this->w += other.w;
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return *this;
}

sm::Quaternion sm::Quaternion::operator* (sm::real scalar) const
{
    return sm::Quaternion(
        this->w * scalar,
        this->x * scalar,
        this->y * scalar,
        this->z * scalar
    );
}

sm::Quaternion sm::Quaternion::conjugate() const
{
    return Quaternion(this->w, -this->x, -this->y, -this->z);
}

sm::real sm::Quaternion::dot(const sm::Quaternion &other) const
{
    return (this->w * other.w) + (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
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

sm::Matrix3x3 sm::Quaternion::toMatrix3x3() const
{
    sm::Matrix3x3 result;

    result[0][0] = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (y * y + z * z);
    result[0][1] = static_cast<sm::real>(2) * (x * y - w * z);
    result[0][2] = static_cast<sm::real>(2) * (x * z + w * y);

    result[1][0] = static_cast<sm::real>(2) * (x * y + w * z);
    result[1][1] = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (x * x + z * z); 
    result[1][2] = static_cast<sm::real>(2) * (y * z - w * x);

    result[2][0] = static_cast<sm::real>(2) * (x * z - w * y);
    result[2][1] = static_cast<sm::real>(2) * (y * z + w * x);
    result[2][2] = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (x * x + y * y);

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