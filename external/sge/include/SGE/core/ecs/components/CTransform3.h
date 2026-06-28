#ifndef SGE_CTRANSFORM3_H
#define SGE_CTRANSFORM3_H
#include <numbers>
#include <sm/Vec3.h>

namespace sge {
/**
 * Component that stores transform data for a 3D environment.
 */
class CTransform3 {
public:
    sm::Vec3 position = {0,0,0};
    sm::Vec3 rotation = {0,0,0};

    /**
     * @return the forward vector of the CTransform3
     */
    sm::Vec3 getForward() const;
};
} // namespace sge

#endif // SGE_CTRANSFORM3_H