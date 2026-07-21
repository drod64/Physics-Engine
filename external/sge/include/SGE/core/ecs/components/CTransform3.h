#ifndef SGE_CTRANSFORM3_H
#define SGE_CTRANSFORM3_H
#include <numbers>
#include <sm/Vec3.h>
#include <sm/Quaternion.h>

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

    /**
     * @return the forward vector of the CTransform3
     */
    sm::Vec3 getForward() const;
};
} // namespace sge

#endif // SGE_CTRANSFORM3_H