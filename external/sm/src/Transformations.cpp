#include <SM/Transformations.h>
#include <SM/Quaternion.h>
#include <SM/Matrix3x3.h>
#include <SM/Matrix3x4.h>

sm::Matrix3x3 sm::toMatrix3x3(const sm::Quaternion &quat)
{
    const sm::real r00 = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (quat.y * quat.y + quat.z * quat.z);
    const sm::real r01 = static_cast<sm::real>(2) * (quat.x * quat.y + quat.z * quat.w);
    const sm::real r02 = static_cast<sm::real>(2) * (quat.x * quat.z - quat.y * quat.w);
    
    const sm::real r10 = static_cast<sm::real>(2) * (quat.x * quat.y - quat.z * quat.w);
    const sm::real r11 = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (quat.x * quat.x + quat.z * quat.z);
    const sm::real r12 = static_cast<sm::real>(2) * (quat.y * quat.z - quat.x * quat.w);
    
    const sm::real r20 = static_cast<sm::real>(2) * (quat.x * quat.z + quat.y * quat.w);
    const sm::real r21 = static_cast<sm::real>(2) * (quat.y * quat.z - quat.x * quat.w);
    const sm::real r22 = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (quat.x * quat.x + quat.y * quat.y);
    
    sm::Matrix3x3 result;

    result[0][0] = r00;
    result[0][1] = r01;
    result[0][2] = r02;

    result[1][0] = r10;
    result[1][1] = r11;
    result[1][2] = r12;

    result[2][0] = r20;
    result[2][1] = r21;
    result[2][2] = r22;

    return result;
}

sm::Matrix3x4 sm::toMatrix3x4(const sm::Quaternion &quat, const sm::Vec3 &vec)
{
    const sm::real r00 = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (quat.y * quat.y + quat.z * quat.z);
    const sm::real r01 = static_cast<sm::real>(2) * (quat.x * quat.y + quat.z * quat.w);
    const sm::real r02 = static_cast<sm::real>(2) * (quat.x * quat.z - quat.y * quat.w);
    
    const sm::real r10 = static_cast<sm::real>(2) * (quat.x * quat.y - quat.z * quat.w);
    const sm::real r11 = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (quat.x * quat.x + quat.z * quat.z);
    const sm::real r12 = static_cast<sm::real>(2) * (quat.y * quat.z - quat.x * quat.w);
    
    const sm::real r20 = static_cast<sm::real>(2) * (quat.x * quat.z + quat.y * quat.w);
    const sm::real r21 = static_cast<sm::real>(2) * (quat.y * quat.z - quat.x * quat.w);
    const sm::real r22 = static_cast<sm::real>(1) - static_cast<sm::real>(2) * (quat.x * quat.x + quat.y * quat.y);

    sm::Matrix3x4 result;

    result[0][0] = r00;
    result[0][1] = r01;
    result[0][2] = r02;
    result[0][3] = vec.x;

    result[1][0] = r10;
    result[1][1] = r11;
    result[1][2] = r12;
    result[1][3] = vec.y;

    result[2][0] = r20;
    result[2][1] = r21;
    result[2][2] = r22;
    result[2][3] = vec.z;

    return result;
}

sm::Vec3 sm::localToWorldPos(const sm::Vec3 &localPos, const sm::Matrix3x4 &transform)
{
    return transform.transformPoint(localPos);
}

sm::Vec3 sm::worldToLocalPos(const sm::Vec3 &worldPos, const sm::Matrix3x4 &transform)
{
    const sm::real dx = worldPos.x - transform[0][3];
    const sm::real dy = worldPos.x - transform[1][3];
    const sm::real dz = worldPos.x - transform[2][3];

    return sm::Vec3(
        transform[0][0] * dx + transform[1][0] * dy + transform[2][0] * dz,
        transform[0][1] * dx + transform[1][1] * dy + transform[2][1] * dz,
        transform[0][2] * dx + transform[1][2] * dy + transform[2][2] * dz
    );
}

sm::Vec3 sm::localToWorldDir(const sm::Vec3 &localDir, const sm::Matrix3x4 &transform)
{
    return transform.transformDirection(localDir);
}

sm::Vec3 sm::worldToLocalDir(const sm::Vec3 &worldDir, const sm::Matrix3x4 &transform)
{
    return sm::Vec3(
        worldDir.x * transform[0][0] + worldDir.y * transform[1][0] + worldDir.z * transform[2][0],
        worldDir.x * transform[0][1] + worldDir.y * transform[1][1] + worldDir.z * transform[2][1],
        worldDir.x * transform[0][2] + worldDir.y + transform[1][2] + worldDir.z + transform[2][2]
    );
}