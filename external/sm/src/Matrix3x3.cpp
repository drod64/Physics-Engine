#include <SM/Matrix3x3.h>

sm::real* sm::Matrix3x3::operator[](uint32_t row)
{
    return &this->m_matrix[row * 3];
}

const sm::real* sm::Matrix3x3::operator[](uint32_t row) const
{
    return &this->m_matrix[row * 3];
}

sm::Matrix3x3 sm::Matrix3x3::operator+(const sm::Matrix3x3 &other) const
{
    sm::Matrix3x3 result;

    result[0][0] = (*this)[0][0] + other[0][0];
    result[0][1] = (*this)[0][1] + other[0][1];
    result[0][2] = (*this)[0][2] + other[0][2];

    result[1][0] = (*this)[1][0] + other[1][0];
    result[1][1] = (*this)[1][1] + other[1][1];
    result[1][2] = (*this)[1][2] + other[1][2];

    result[2][0] = (*this)[2][0] + other[2][0];
    result[2][1] = (*this)[2][1] + other[2][1];
    result[2][2] = (*this)[2][2] + other[2][2];

    return result;
}

sm::Vec3 sm::Matrix3x3::operator*(const sm::Vec3 &v) const
{
    return Vec3(
        this->m_matrix[0] * v.x + this->m_matrix[1] * v.y + this->m_matrix[2] * v.z,
        this->m_matrix[3] * v.x + this->m_matrix[4] * v.y + this->m_matrix[5] * v.z,
        this->m_matrix[6] * v.x + this->m_matrix[7] * v.y + this->m_matrix[8] * v.z
    );
}

sm::Matrix3x3 sm::Matrix3x3::operator*(const sm::Matrix3x3 &other) const
{
    sm::Matrix3x3 result;

    result[0][0] = (*this)[0][0] * other[0][0] + (*this)[0][1] * other[1][0] + (*this)[0][2] * other[2][0];
    result[0][1] = (*this)[0][0] * other[0][1] + (*this)[0][1] * other[1][1] + (*this)[0][2] * other[2][1];
    result[0][2] = (*this)[0][0] * other[0][2] + (*this)[0][1] * other[1][2] + (*this)[0][2] * other[2][2];

    result[1][0] = (*this)[1][0] * other[0][0] + (*this)[1][1] * other[1][0] + (*this)[1][2] * other[2][0];
    result[1][1] = (*this)[1][0] * other[0][1] + (*this)[1][1] * other[1][1] + (*this)[1][2] * other[2][1];
    result[1][2] = (*this)[1][0] * other[0][2] + (*this)[1][1] * other[1][2] + (*this)[1][2] * other[2][2];

    result[2][0] = (*this)[2][0] * other[0][0] + (*this)[2][1] * other[1][0] + (*this)[2][2] * other[2][0];
    result[2][1] = (*this)[2][0] * other[0][1] + (*this)[2][1] * other[1][1] + (*this)[2][2] * other[2][1];
    result[2][2] = (*this)[2][0] * other[0][2] + (*this)[2][1] * other[1][2] + (*this)[2][2] * other[2][2];

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::operator*(sm::real scalar) const
{
    sm::Matrix3x3 result;

    result[0][0] = (*this)[0][0] * scalar;
    result[0][1] = (*this)[0][1] * scalar;
    result[0][2] = (*this)[0][2] * scalar;

    result[1][0] = (*this)[1][0] * scalar;
    result[1][1] = (*this)[1][1] * scalar;
    result[1][2] = (*this)[1][2] * scalar;

    result[2][0] = (*this)[2][0] * scalar;
    result[2][1] = (*this)[2][1] * scalar;
    result[2][2] = (*this)[2][2] * scalar;

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::transpose() const
{
    sm::Matrix3x3 result;

    result[0][0] = (*this)[0][0];
    result[0][1] = (*this)[1][0];
    result[0][2] = (*this)[2][0];
    
    result[1][0] = (*this)[0][1];
    result[1][1] = (*this)[1][1];
    result[1][2] = (*this)[2][1];
    
    result[2][0] = (*this)[0][2];
    result[2][1] = (*this)[1][2];
    result[2][2] = (*this)[2][2];

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::inverse() const
{
    sm::Matrix3x3 result;

    sm::real det = (*this)[0][0] * ((*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1]) -
                   (*this)[0][1] * ((*this)[1][0] * (*this)[2][2] - (*this)[1][2] * (*this)[2][0]) +
                   (*this)[0][2] * ((*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0]);

    if (real_abs(det) < static_cast<sm::real>(1e-6)) return sm::Matrix3x3();

    float invDet = static_cast<sm::real>(1) / det;

    result[0][0] = ((*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1]) * invDet;
    result[0][1] = ((*this)[0][2] * (*this)[2][1] - (*this)[0][1] * (*this)[2][2]) * invDet;
    result[0][2] = ((*this)[0][1] * (*this)[1][2] - (*this)[0][2] * (*this)[1][1]) * invDet;

    result[1][0] = ((*this)[1][2] * (*this)[2][0] - (*this)[1][0] * (*this)[2][2]) * invDet;
    result[1][1] = ((*this)[0][0] * (*this)[2][2] - (*this)[0][2] * (*this)[2][0]) * invDet;
    result[1][2] = ((*this)[0][2] * (*this)[1][0] - (*this)[0][0] * (*this)[1][2]) * invDet;

    result[2][0] = ((*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0]) * invDet;
    result[2][1] = ((*this)[0][1] * (*this)[2][0] - (*this)[0][0] * (*this)[2][1]) * invDet;
    result[2][2] = ((*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0]) * invDet;

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::absolute() const
{
    sm::Matrix3x3 result;

    result[0][0] = real_abs((*this)[0][0]);
    result[0][1] = real_abs((*this)[0][1]);
    result[0][2] = real_abs((*this)[0][2]);

    result[1][0] = real_abs((*this)[1][0]);
    result[1][1] = real_abs((*this)[1][1]);
    result[1][2] = real_abs((*this)[1][2]);

    result[2][0] = real_abs((*this)[2][0]);
    result[2][1] = real_abs((*this)[2][1]);
    result[2][2] = real_abs((*this)[2][2]);

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::outerProduct(const sm::Vec3 &v1, const sm::Vec3 &v2)
{
    sm::Matrix3x3 result;

    result[0][0] = v1.x * v2.x;
    result[0][1] = v1.x * v2.y;
    result[0][2] = v1.x * v2.z;

    result[1][0] = v1.y * v2.x;
    result[1][1] = v1.y * v2.y;
    result[1][2] = v1.y * v2.z;

    result[2][0] = v1.z * v2.x;
    result[2][1] = v1.z * v2.y;
    result[2][2] = v1.z * v2.z;

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::createSkewSymmetric(const sm::Vec3 &v)
{
    sm::Matrix3x3 result;

    result[0][0] = 0;
    result[0][1] = -v.z;
    result[0][2] = v.y;

    result[1][0] = v.z;
    result[1][1] = 0;
    result[1][2] = -v.x;

    result[2][0] = -v.y;
    result[2][1] = v.x;
    result[2][2] = 0;

    return result;
}

sm::real* sm::Matrix3x3::data()
{
    return this->m_matrix;
}

const sm::real* sm::Matrix3x3::data() const
{
    return this->m_matrix;
}