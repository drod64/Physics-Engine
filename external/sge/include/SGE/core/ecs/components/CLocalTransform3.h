#ifndef SGE_CLOCAL_TRANSFORM3_H
#define SGE_CLOCAL_TRANSFORM3_H
#include <SM/Vec3.h>
#include <SM/Quaternion.h>

namespace sge {
    struct CLocalTransform3 {
        sm::Vec3 position;
        sm::Quaternion orientation;
        sm::Vec3 scale = {static_cast<sm::real>(1), static_cast<sm::real>(1), static_cast<sm::real>(1)};
    }; // struct CLocalTransform3
} // namespace sge

#endif // SGE_CLOCAL_TRANSFORM3_H