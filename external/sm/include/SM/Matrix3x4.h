#ifndef SM_MATRIX_3X4_H
#define SM_MATRIX_3X4_H
#include <SM/Vec3.h>

namespace sm {
class Matrix3x4 {
private:
    real m_matrix[12] = {1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0};

public:
    real* operator[](size_t row);
    const real* operator[](size_t row) const;

}; // class Matrix3x4

// Math operator overload declarations.
Matrix3x4 operator+(const Matrix3x4 &lhs, const Matrix3x4 &rhs);
Matrix3x4 operator*(const Matrix3x4 &lhs, const Matrix3x4 &rhs);
Matrix3x4 operator*(const Matrix3x4 &mat, real scalar);
Matrix3x4 operator*(real scalar, const Matrix3x4 &mat);
Vec3 operator*(const Matrix3x4 &mat, const Vec3 &v);
Vec3 operator*(const Vec3 &v, const Matrix3x4 &mat);

} // namespace sm

// Math operator overload implementations.
inline sm::Matrix3x4 sm::operator+(const sm::Matrix3x4 &lhs, const sm::Matrix3x4 &rhs)
{
    sm::Matrix3x4 result;

    result[0][0] = lhs[0][0] + rhs[0][0];
    result[0][1] = lhs[0][1] + rhs[0][1];
    result[0][2] = lhs[0][2] + rhs[0][2];
    result[0][3] = lhs[0][3] + rhs[0][3];

    result[1][0] = lhs[1][0] + rhs[1][0];
    result[1][1] = lhs[1][1] + rhs[1][1];
    result[1][2] = lhs[1][2] + rhs[1][2];
    result[1][3] = lhs[1][3] + rhs[1][3];

    result[2][0] = lhs[2][0] + rhs[2][0];
    result[2][1] = lhs[2][1] + rhs[2][1];
    result[2][2] = lhs[2][2] + rhs[2][2];
    result[2][3] = lhs[2][3] + rhs[2][3];

    return result;
}

inline sm::Matrix3x4 sm::operator*(const sm::Matrix3x4 &lhs, const sm::Matrix3x4 &rhs)
{
    sm::Matrix3x4 result;

    result[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0]; // + 0
    result[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1]; // + 0
    result[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2]; // + 0
    result[0][3] = lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3] + lhs[0][3];

    result[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0]; // + 0
    result[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1]; // + 0
    result[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2]; // + 0
    result[1][3] = lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3] + lhs[1][3];
    
    result[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0]; // + 0
    result[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1]; // + 0
    result[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2]; // + 0
    result[2][3] = lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3] + lhs[2][3];

    return result;
}

inline sm::Matrix3x4 sm::operator*(const sm::Matrix3x4 &mat, sm::real scalar)
{
    sm::Matrix3x4 result;

    result[0][0] = mat[0][0] * scalar;
    result[0][1] = mat[0][1] * scalar;
    result[0][2] = mat[0][2] * scalar;
    result[0][3] = mat[0][3] * scalar;

    result[1][0] = mat[1][0] * scalar;
    result[1][1] = mat[1][1] * scalar;
    result[1][2] = mat[1][2] * scalar;
    result[1][3] = mat[1][3] * scalar;

    result[2][0] = mat[2][0] * scalar;
    result[2][1] = mat[2][1] * scalar;
    result[2][2] = mat[2][2] * scalar;
    result[2][3] = mat[2][3] * scalar;

    return result;
}

inline sm::Matrix3x4 sm::operator*(sm::real scalar, const sm::Matrix3x4 &mat)
{
    return mat * scalar;
}

inline sm::Vec3 sm::operator*(const sm::Matrix3x4 &mat, const sm::Vec3 &v)
{
    return sm::Vec3 (
        mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3], // * 1
        mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3], // * 1
        mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3]  // * 1
    );
}

inline sm::Vec3 sm::operator*(const sm::Vec3 &v, const sm::Matrix3x4 &mat)
{
    return mat * v;
}

#endif // SM_MATRIX_3x4_H