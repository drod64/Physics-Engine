#ifndef SGE_RIGID_BODY_UTILS_H
#define SGE_RIGID_BODY_UTILS_H
#include <SM/Vec3.h>
#include <SGE/physics/forceComponents/CRigidBody3.h>
#include <SGE/core/ecs/components/CTransform3.h>

namespace sge {
    namespace RigidBodyUtils {
        void addForceAtGlobalPoint(CRigidBody3 &body, const CTransform3 &transform,
                                    const sm::Vec3 &globalForce, const sm::Vec3 &globalPoint);

        void addForceAtLocalPoint(CRigidBody3 &body, const CTransform3 &transform,
                                    const sm::Vec3 &localForce, const sm::Vec3 &localPoint);
    } // namespace RigidBodyUtils
} // namespace sge

#endif // SGE_RIGID_BODY_UTILS_H