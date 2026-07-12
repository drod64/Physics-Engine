#include <SM/Matrix3x4.h>

sm::real* sm::Matrix3x4::operator[](size_t row)
{
    return &this->m_matrix[row];
}

const sm::real* sm::Matrix3x4::operator[](size_t row) const
{
    return &this->m_matrix[row];
}