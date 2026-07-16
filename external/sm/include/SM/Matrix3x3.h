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
    
    /**
     * Sets the internal array to represent a 3x3 matrix.
     * This function simply copies the values from the passed in sm::real*
     * 
     * @param matrix the sm::real* to copy from
     */
    void setMatrix(const sm::real *matrix);

public:
    /**
     * @param row the desired row
     * @return a sm::real pointer offset by row
     */
    sm::real* operator[] (uint32_t row);
    
    /**
     * @param row the desired row
     * @return a sm::real pointer offset by row
     */
    const sm::real* operator[] (uint32_t row) const;
    
    /**
     * Transposes the Matrix3x3 in-place.
     */
    void transpose();
    
    /**
     * Inverses the Matrix3x3 in-place.
     */
    void inverse();
    
    /**
     * Absolutes the Matrix3x3 in-place.
     */
    void absolute();
    
    /**
     * Sets the data of the Matrix3x3.
     * @param mat the Matrix3x3 to copy from
     */
    void setMatrix3x3(const Matrix3x3 &mat);
    
    /**
     * @return the internal sm::real[]
     */
    sm::real* data();
    
    /**
     * @return the const internal sm::real[]
     */
    const sm::real* data() const;

    /**
     * Returns an absolute version of the Matrix3x3.
     * 
     * NOTE: This is a non-modifying function.
     * @return the absolute copy-version of the Matrix3x3
     */
    sm::Matrix3x3 absoluted() const;
    
    /**
     * Returns a transposed version of the Matrix3x3.
     * 
     * NOTE: This is a non-modifying function.
     * @return the transpose copy-version of the Matrix3x3
     */
    sm::Matrix3x3 transposed() const;
    
    /**
     * Returns an inversed version of the Matrix3x3.
     * 
     * NOTE: This is a non-modifying function.
     * @return the inverse copy-version of the Matrix3x3
     */
    sm::Matrix3x3 inversed() const;
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
    const sm::real a = lhs[0][0], b = lhs[0][1], c = lhs[0][2];
    const sm::real d = lhs[1][0], e = lhs[1][1], f = lhs[1][2];
    const sm::real g = lhs[2][0], h = lhs[2][1], i = lhs[2][2];

    const sm::real j = rhs[0][0], k = rhs[0][1], l = rhs[0][2];
    const sm::real m = rhs[1][0], n = rhs[1][1], o = rhs[1][2];
    const sm::real p = rhs[2][0], q = rhs[2][1], r = rhs[2][2];

    sm::Matrix3x3 result;

    result[0][0] = (a * j) + (b * m) + (c * p);
    result[0][1] = (a * k) + (b * n) + (c * q);
    result[0][2] = (a * l) + (b * o) + (c * r);

    result[1][0] = (d * j) + (e * m) + (f * p);
    result[1][1] = (d * k) + (e * n) + (f * q);
    result[1][2] = (d * l) + (e * o) + (f * r);

    result[2][0] = (g * j) + (h * m) + (i * p);
    result[2][1] = (g * k) + (h * n) + (i * q);
    result[2][2] = (g * l) + (h * o) + (i * r);

    return result;
}

inline sm::Vec3 sm::operator*(const sm::Matrix3x3 &mat, const sm::Vec3 &v)
{
    const sm::real x = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z;
    const sm::real y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z;
    const sm::real z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z;

    return sm::Vec3(x, y, z);
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