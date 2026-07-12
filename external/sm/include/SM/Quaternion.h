#ifndef SM_QUATERNION_H
#define SM_QUATERNION_H
#include <SM/Precision.h>
#include <SM/Matrix3x3.h>

namespace sm {
struct Quaternion {
    sm::real w = 1;
    sm::real x = 0;
    sm::real y = 0;
    sm::real z = 0;

    Quaternion(sm::real w, sm::real x, sm::real y, sm::real z);
    
    Quaternion& operator+= (const Quaternion &other);

    Quaternion conjugate() const;

    void normalize();
    
    Matrix3x3 toMatrix3x3() const;

    static sm::real dot(const Quaternion &lhs, const Quaternion &rhs);

    static Quaternion normalized(const Quaternion &quat);

    static Quaternion fromAxisAngle(const Vec3 &axis, real angleRadians);
}; // class Quaternion

// Math operator overload declarations.
Quaternion operator* (const Quaternion &lhs, const Quaternion &rhs);
Vec3 operator* (const Quaternion &quat, const Vec3 &v);
Vec3 operator* (const Vec3 &v, const Quaternion &quat);
Quaternion operator* (const Quaternion &quat, sm::real scalar);
Quaternion operator* (sm::real scalar, const Quaternion &quat);
} // namespace sm

// Math operator overload implementations
inline sm::Quaternion sm::operator* (const sm::Quaternion &lhs, const sm::Quaternion &rhs)
{
    return sm::Quaternion(
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w
        );
}

inline sm::Vec3 sm::operator* (const sm::Quaternion &quat, const sm::Vec3 &v)
{
    sm::Vec3 qVec(quat.x, quat.y, quat.z);

    sm::Vec3 cross1 = sm::Vec3::crossProduct(qVec, v);
    sm::Vec3 cross2 = sm::Vec3::crossProduct(qVec, cross1);

    return v + (cross1 * quat.w + cross2) * static_cast<sm::real>(2);
}

inline sm::Vec3 sm::operator* (const sm::Vec3 &v, const sm::Quaternion &mat)
{
    return mat * v;
}

inline sm::Quaternion sm::operator* (const sm::Quaternion &quat, sm::real scalar)
{
    return sm::Quaternion(
        quat.w * scalar,
        quat.x * scalar,
        quat.y * scalar,
        quat.z * scalar
    );
}

inline sm::Quaternion sm::operator* (sm::real scalar, const sm::Quaternion &quat)
{
    return quat * scalar;
}

#endif // SM_QUATERNION_H