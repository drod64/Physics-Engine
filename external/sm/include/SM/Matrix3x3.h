#ifndef SM_MATRIX_3X3_H
#define SM_MATRIX_3X3_H
#include <cstdint>
#include <SM/Precision.h>
#include <SM/Vec3.h>

namespace sm {
struct Matrix3x3 {
private:
    sm::real m_matrix[9] = {1,0,0,
                            0,1,0,
                            0,0,1};
public:
    sm::real* operator[] (uint32_t row);

    const sm::real* operator[] (uint32_t row) const;

    Matrix3x3 operator+ (const Matrix3x3 &other) const;

    Vec3 operator* (const Vec3 &v) const;

    Matrix3x3 operator* (const Matrix3x3 &other) const;

    Matrix3x3 operator* (sm::real scalar) const;

    Matrix3x3 transpose() const;

    Matrix3x3 inverse() const;

    Matrix3x3 absolute() const;

    static Matrix3x3 outerProduct(const Vec3 &v1, const Vec3 &v2);

    static Matrix3x3 createSkewSymmetric(const Vec3 &v);

    sm::real* data();

    const sm::real* data() const;

}; // class Matrix3x3
} // namespace sm

#endif // SM_MATRIX_3X3_H