#ifndef SM_TRANSFORMATIONS_H
#define SM_TRANSFORMATIONS_H

namespace sm {
struct Quaternion;
struct Matrix3x3;
struct Matrix3x4;
struct Vec3;

    /**
     * Converts a Quaternion to a Matrix3x3.
     * @param quat a sm::Quaternion
     * @return a sm::Matrix3x3
     */
    Matrix3x3 toMatrix3x3(const Quaternion &quat);

    /**
     * Combines a Quaternion and Vec3 into a singular Matrix3x4.
     * @param quat a sm::Quaternion
     * @param vec a sm::Vec3
     * @return a sm::Matrix3x4
     */
    Matrix3x4 toMatrix3x4(const Quaternion &quat, const Vec3 &vec);
} // namespace sm

#endif // SM_TRANSFORMATIONS_H