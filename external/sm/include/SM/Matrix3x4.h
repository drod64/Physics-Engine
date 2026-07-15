#ifndef SM_MATRIX_3X4_H
#define SM_MATRIX_3X4_H
#include <stdexcept>
#include <SM/Vec3.h>

namespace sm {
/**
 * This Matrix actually only holds 12 elements. However, it implicity uses [0,0,0,1] as the last row for
 * a 4x4 Matrix effect.
 */
class Matrix3x4 {
private:
    real m_matrix[12] = {1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0};

    void setMatrix(const real *matrix);
public:
    /**
     * @param row the desired row
     * @return a sm::real pointer offset by row
     */
    real* operator[](size_t row);

    /**
     * @param row the desired row
     * @return a const sm::real pointer offset by row
     */
    const real* operator[](size_t row) const;
    
    /**
     * Inverses the Matrix3x4 in-place.
     */
    void inverse();
    
    /**
     * Sets the data of the Matrix3x4.
     * @mat the sm::Matrix3x4 to copy from
     */
    void setMatrix3x4(const Matrix3x4 &mat);
    
    /**
     * @return the internal sm::real[]
     */
    sm::real* data();

    /**
     * @return the const internal sm::real[]
     */
    const sm::real *data() const;
    
    //////////////////////////
    //   Static functions   //
    //////////////////////////

    /**
     * Static helper.
     * @param mat the Matrix3x4
     * @return the inverse copy-version of the Matrix3x4
     */
    static sm::Matrix3x4 inversed(const Matrix3x4 &mat);
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
    const sm::real a = lhs[0][0], b = lhs[0][1], c = lhs[0][2], d = lhs[0][3];
    const sm::real e = lhs[1][0], f = lhs[1][1], g = lhs[1][2], h = lhs[1][3];
    const sm::real i = lhs[2][0], j = lhs[2][1], k = lhs[2][2], l = lhs[2][3];

    const sm::real m = rhs[0][0], n = rhs[0][1], o = rhs[0][2], p = rhs[0][3];
    const sm::real q = rhs[1][0], r = rhs[1][1], s = rhs[1][2], t = rhs[1][3];
    const sm::real u = rhs[2][0], v = rhs[2][1], w = rhs[2][2], x = rhs[2][3];

    sm::Matrix3x4 result;

    result[0][0] = (a * m) + (b * q) + (c * u); // + 0
    result[0][1] = (a * n) + (b * r) + (c * v); // + 0
    result[0][2] = (a * o) + (b * s) + (c * w); // + 0
    result[0][3] = (a * p) + (b * t) + (c * x) + d;

    result[1][0] = (e * m) + (f * q) + (g * u); // + 0
    result[1][1] = (e * n) + (f * r) + (g * v); // + 0
    result[1][2] = (e * o) + (f * s) + (g * w); // + 0
    result[1][3] = (e * p) + (f * t) + (g * x) + h;
    
    result[2][0] = (i * m) + (j * q) + (k * u); // + 0
    result[2][1] = (i * n) + (j * r) + (k * v); // + 0
    result[2][2] = (i * o) + (j * s) + (k * w); // + 0
    result[2][3] = (i * p) + (j * t) + (k * x) + l;

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
    const sm::real x = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3]; // * 1
    const sm::real y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3]; // * 1
    const sm::real z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3]; // * 1

    return sm::Vec3 (x, y, z);
}

inline sm::Vec3 sm::operator*(const sm::Vec3 &v, const sm::Matrix3x4 &mat)
{
    return mat * v;
}

#endif // SM_MATRIX_3x4_H