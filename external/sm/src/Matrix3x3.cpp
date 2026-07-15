#include <SM/Matrix3x3.h>

void sm::Matrix3x3::setMatrix(const sm::real *matrix)
{
    this->m_matrix[0] = matrix[0];
    this->m_matrix[1] = matrix[1];
    this->m_matrix[2] = matrix[2];

    this->m_matrix[3] = matrix[3];
    this->m_matrix[4] = matrix[4];
    this->m_matrix[5] = matrix[5];

    this->m_matrix[6] = matrix[6];
    this->m_matrix[7] = matrix[7];
    this->m_matrix[8] = matrix[8];
}

sm::real* sm::Matrix3x3::operator[](uint32_t row)
{
    return &this->m_matrix[row * 3];
}

const sm::real* sm::Matrix3x3::operator[](uint32_t row) const
{
    return &this->m_matrix[row * 3];
}

void sm::Matrix3x3::transpose()
{
    const sm::real b = this->m_matrix[1], c = this->m_matrix[2];
    const sm::real d = this->m_matrix[3], f = this->m_matrix[5];
    const sm::real g = this->m_matrix[6], h = this->m_matrix[7];

    // a, e, i do not not change position.

    this->m_matrix[1] = d;
    this->m_matrix[2] = g;

    this->m_matrix[3] = b;
    this->m_matrix[5] = h;

    this->m_matrix[6] = c;
    this->m_matrix[7] = f;
}

void sm::Matrix3x3::inverse()
{
    const sm::real a = this->m_matrix[0], b = this->m_matrix[1], c = this->m_matrix[2];
    const sm::real d = this->m_matrix[3], e = this->m_matrix[4], f = this->m_matrix[5];
    const sm::real g = this->m_matrix[6], h = this->m_matrix[7], i = this->m_matrix[8];

    const sm::real c11 = e * i - f * h;
    const sm::real c21 = f * g - d * i;
    const sm::real c31 = d * h - e * g;

    const sm::real det = a * c11 + b * c21 + c * c31;

    if (real_abs(det) < static_cast<sm::real>(1e-6))
    {
        throw std::runtime_error("[Matrix3x3]::inversed: Error. Matrix is singular.");
    }

    const sm::real invDet = static_cast<sm::real>(1) / det;

    const sm::real r00 = c11 * invDet;
    const sm::real r01 = (c * h - b * i) * invDet;
    const sm::real r02 = (b * f - c * e) * invDet;
    
    const sm::real r10 = c21 * invDet;
    const sm::real r11 = (a * i - c * g) * invDet;
    const sm::real r12 = (c * d - a * f) * invDet;
    
    const sm::real r20 = c31 * invDet;
    const sm::real r21 = (b * g - a * h) * invDet;
    const sm::real r22 = (a * e - b * d) * invDet;

    this->m_matrix[0] = r00;
    this->m_matrix[1] = r01;
    this->m_matrix[2] = r02;

    this->m_matrix[3] = r10;
    this->m_matrix[4] = r11;
    this->m_matrix[5] = r12;

    this->m_matrix[6] = r20;
    this->m_matrix[7] = r21;
    this->m_matrix[8] = r22;
}

void sm::Matrix3x3::absolute()
{
    (*this)[0][0] = real_abs((*this)[0][0]);
    (*this)[0][1] = real_abs((*this)[0][1]);
    (*this)[0][2] = real_abs((*this)[0][2]);

    (*this)[1][0] = real_abs((*this)[1][0]);
    (*this)[1][1] = real_abs((*this)[1][1]);
    (*this)[1][2] = real_abs((*this)[1][2]);

    (*this)[2][0] = real_abs((*this)[2][0]);
    (*this)[2][1] = real_abs((*this)[2][1]);
    (*this)[2][2] = real_abs((*this)[2][2]);
}

sm::real* sm::Matrix3x3::data()
{
    return this->m_matrix;
}

const sm::real* sm::Matrix3x3::data() const
{
    return this->m_matrix;
}

sm::Matrix3x3 sm::Matrix3x3::absoluted(const sm::Matrix3x3 &mat)
{
    sm::Matrix3x3 result;

    result.setMatrix(mat.data());

    result.absolute();

    return result;
}

sm::Matrix3x3 sm::Matrix3x3::transposed(const sm::Matrix3x3 &mat)
{
    sm::Matrix3x3 result;

    result.setMatrix(mat.data());

    result.transpose();
    
    return result;
}

sm::Matrix3x3 sm::Matrix3x3::inversed(const sm::Matrix3x3 &mat)
{
    sm::Matrix3x3 result;

    result.setMatrix(mat.data());

    result.inverse();

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