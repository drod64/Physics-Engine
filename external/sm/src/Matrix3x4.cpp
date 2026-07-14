#include <SM/Matrix3x4.h>

void sm::Matrix3x4::setMatrix(const sm::real *matrix)
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
    this->m_matrix[9] = matrix[9];
    this->m_matrix[10] = matrix[10];
    this->m_matrix[11] = matrix[11];
}

sm::real* sm::Matrix3x4::operator[](size_t row)
{
    return &this->m_matrix[row];
}

const sm::real* sm::Matrix3x4::operator[](size_t row) const
{
    return &this->m_matrix[row];
}

void sm::Matrix3x4::inverse()
{
    const sm::real a = this->m_matrix[0], b = this->m_matrix[1], c = this->m_matrix[2];
    const sm::real d = this->m_matrix[4], e = this->m_matrix[5], f = this->m_matrix[6];
    const sm::real g = this->m_matrix[8], h = this->m_matrix[9], i = this->m_matrix[10];

    const sm::real tx = this->m_matrix[3], ty = this->m_matrix[7], tz = this->m_matrix[11];

    const sm::real c11 = e * i - f * h;
    const sm::real c21 = f * g - d * i;
    const sm::real c31 = d * h - e * g;

    const sm::real det = a * c11 + b * c21 + c * c31;

    if (real_abs(det) < static_cast<sm::real>(1e-6))
    {
        throw std::runtime_error("[Matrix3x4]::inversed: Error. Matrix is singular.");
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

    const sm::real itx = -(r00 * tx + r01 * ty + r02 * tz);
    const sm::real ity = -(r10 * tx + r11 * ty + r12 * tz);
    const sm::real itz = -(r20 * tx + r21 * ty + r22 * tz);

    this->m_matrix[0] = r00;
    this->m_matrix[1] = r01;
    this->m_matrix[2] = r02;
    this->m_matrix[3] = itx;

    this->m_matrix[4] = r10;
    this->m_matrix[5] = r11;
    this->m_matrix[6] = r12;
    this->m_matrix[7] = ity;

    this->m_matrix[8] = r20;
    this->m_matrix[9] = r21;
    this->m_matrix[10] = r22;
    this->m_matrix[11] = itz;
}

sm::real* sm::Matrix3x4::data()
{
    return this->m_matrix;
}

const sm::real* sm::Matrix3x4::data() const
{
    return this->m_matrix;
}

sm::Matrix3x4 sm::Matrix3x4::inversed(const sm::Matrix3x4 &mat)
{
    sm::Matrix3x4 result;

    result.setMatrix(mat.data());
    
    result.inverse();

    return result;
}