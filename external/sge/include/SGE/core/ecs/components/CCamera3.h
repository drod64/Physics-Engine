#ifndef SGE_CCAMERA3_H
#define SGE_CCAMERA3_H
#include <SM/Vec3.h>

namespace sge {
/**
 * Camera component for a 3D environment.
 */
class CCamera3 {
public:
    sm::real fov;
    sm::Vec3 up;
    bool isActive;
    int projection;

    /**
     * Parameterized constructor.
     * @param isActive sets the camera as active (true or false)
     * @param projectionType the desired projection type (usually an enum)
     * @param fov the field of view of the camera
     * @param up a sm::Vec3 that tells the camera which way is up
     */
    CCamera3(bool isActive, int projectionType, sm::real fov = 65.f, const sm::Vec3 &up = {0.f, 1.f, 0.f});
};
} // namespace sge

#endif // SGE_CCAMERA3_H