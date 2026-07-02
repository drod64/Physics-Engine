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

    Quaternion operator* (const Quaternion &other) const;

    Vec3 operator* (const Vec3 &v) const;
    
    Quaternion& operator+= (const Quaternion &other);

    Quaternion operator* (sm::real scalar) const;

    Quaternion conjugate() const;

    sm::real dot(const Quaternion &other) const;

    void normalize();

    Matrix3x3 toMatrix3x3() const;

    static Quaternion fromAxisAngle(const Vec3 &axis, real angleRadians);
}; // class Quaternion
} // namespace sm

#endif // SM_QUATERNION_H