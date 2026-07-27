#ifndef SGE_CTRANSFORM3_H
#define SGE_CTRANSFORM3_H
#include <sm/Vec3.h>
#include <sm/Quaternion.h>
#include <SGE/SGEConfig.h>

namespace sge {
/**
 * Component that stores transform data for a 3D environment.
 */
class CTransform3 {
public:
    sm::Vec3 position;
    sm::Vec3 prevPosition;
    sm::Quaternion orientation;
    sm::Quaternion prevOrientation;

    inline sm::Vec3 forward() const
    {
        return orientation * sge::Axes::GLOBAL_FORWARD;
    }

    inline sm::Vec3 up() const
    {
        return orientation * sge::Axes::GLOBAL_UP;
    }

    inline sm::Vec3 right() const
    {
        return orientation * sge::Axes::GLOBAL_RIGHT;
    }

    inline void translate(const sm::Vec3 &direction, sm::real distance)
    {
        position += direction * distance;
    }
};
} // namespace sge

#endif // SGE_CTRANSFORM3_H