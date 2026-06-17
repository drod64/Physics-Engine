#ifndef SGE_CTRANSFORM3_H
#define SGE_CTRANSFORM3_H
#include <sm/Vec3.h>

namespace sge {
/**
 * Component that stores transform data for a 3D environment.
 */
class CTransform3 {
public:
    sm::Vec3 position;
    sm::Vec3 rotation;

    /**
     * Default Constructor.
     */
    CTransform3();
};
} // namespace sge

#endif // SGE_CTRANSFORM3_H