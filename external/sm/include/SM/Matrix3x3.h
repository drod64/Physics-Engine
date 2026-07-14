#ifndef SM_MATRIX_3X3_H
#define SM_MATRIX_3X3_H
#include <cstdint>
#include <stdexcept>
#include <SM/Precision.h>
#include <SM/Vec3.h>

namespace sm {
struct Matrix3x3 {
private:
    sm::real m_matrix[9] = {1,0,0,
                            0,1,0,
                            0,0,1};

    void setMatrix(const sm::real *matrix);

public:
    sm::real* operator[] (uint32_t row);

    const sm::real* operator[] (uint32_t row) const;

    void transpose();

    void inverse();

    Matrix3x3 absolute() const;

    static Matrix3x3 transposed(const Matrix3x3 &mat);

    static Matrix3x3 inversed(const Matrix3x3 &mat);

    static Matrix3x3 outerProduct(const Vec3 &v1, const Vec3 &v2);

    static Matrix3x3 createSkewSymmetric(const Vec3 &v);

    sm::real* data();

    const sm::real* data() const;

}; // class Matrix3x3

// Math operator overload declarations
Matrix3x3 operator+ (const Matrix3x3 &lhs, const Matrix3x3 &rhs);
Matrix3x3 operator* (const Matrix3x3 &lhs, const Matrix3x3 &rhs);
Vec3 operator* (const Matrix3x3 &mat, const Vec3 &v);
Vec3 operator* (const Vec3 &v, const Matrix3x3 &mat);
Matrix3x3 operator* (const Matrix3x3 &mat, real scalar);
Matrix3x3 operator* (real scalar, const Matrix3x3 &mat);
} // namespace sm

// Math operator overload implementations
inline sm::Matrix3x3 sm::operator+(const sm::Matrix3x3 &lhs, const sm::Matrix3x3 &rhs)
{
    sm::Matrix3x3 result;

    result[0][0] = lhs[0][0] + rhs[0][0];
    result[0][1] = lhs[0][1] + rhs[0][1];
    result[0][2] = lhs[0][2] + rhs[0][2];

    result[1][0] = lhs[1][0] + rhs[1][0];
    result[1][1] = lhs[1][1] + rhs[1][1];
    result[1][2] = lhs[1][2] + rhs[1][2];

    result[2][0] = lhs[2][0] + rhs[2][0];
    result[2][1] = lhs[2][1] + rhs[2][1];
    result[2][2] = lhs[2][2] + rhs[2][2];

    return result;
}

inline sm::Matrix3x3 sm::operator*(const sm::Matrix3x3 &lhs, const sm::Matrix3x3 &rhs)
{
    sm::Matrix3x3 result;

    result[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0];
    result[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1];
    result[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2];

    result[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0];
    result[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1];
    result[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2];

    result[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0];
    result[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1];
    result[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2];

    return result;
}

inline sm::Vec3 sm::operator*(const sm::Matrix3x3 &mat, const sm::Vec3 &v)
{
    return sm::Vec3(
        mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z,
        mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z,
        mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z
    );
}

inline sm::Vec3 sm::operator* (const sm::Vec3 &v, const sm::Matrix3x3 &mat)
{
    return mat * v;
}

inline sm::Matrix3x3 sm::operator*(const sm::Matrix3x3 &mat, sm::real scalar)
{
    sm::Matrix3x3 result;

    result[0][0] = mat[0][0] * scalar;
    result[0][1] = mat[0][1] * scalar;
    result[0][2] = mat[0][2] * scalar;

    result[1][0] = mat[1][0] * scalar;
    result[1][1] = mat[1][1] * scalar;
    result[1][2] = mat[1][2] * scalar;

    result[2][0] = mat[2][0] * scalar;
    result[2][1] = mat[2][1] * scalar;
    result[2][2] = mat[2][2] * scalar;

    return result;
}
inline sm::Matrix3x3 sm::operator*(sm::real scalar, const sm::Matrix3x3 &mat)
{
    return mat * scalar;
}

#endif // SM_MATRIX_3X3_H