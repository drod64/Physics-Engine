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

    /**
     * Returns a global position.
     * @param localPos the local position
     * @param transform the Matrix3x4 transform
     * @return a global position
     */
    Vec3 localToWorldPos(const Vec3 &localPos, const Matrix3x4 &transform);

    /**
     * Returns a local position of a transform.
     * @param worldPos the world position
     * @param transform the Matrix3x4 transform
     * @return a local position
     */
    Vec3 worldToLocalPos(const Vec3 &worldPos, const Matrix3x4 &transform);

    /**
     * Returns a global direction.
     * @param localDir the local direction
     * @param transform the Matrix3x4 transform
     * @return a global direction
     */
    Vec3 localToWorldDir(const Vec3 &localDir, const Matrix3x4 &transform);

    /**
     * Returns a local direction of a transform.
     * @param worldDir the world direction
     * @param transform the Matrix3x4 transform
     * @return a local direction
     */
    Vec3 worldToLocalDir(const Vec3 &worldDir, const Matrix3x4 &transform);
} // namespace sm

#endif // SM_TRANSFORMATIONS_H