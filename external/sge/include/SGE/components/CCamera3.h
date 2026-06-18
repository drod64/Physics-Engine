#ifndef SGE_CCAMERA3_H
#define SGE_CCAMERA3_H
#include <SM/Vec3.h>

namespace sge {
class CCamera3 {
public:
    sm::real fov;
    sm::Vec3 up;
    bool isActive;
    int projection;

    CCamera3(bool isActive, int projectionType, sm::real fov = 65.f, const sm::Vec3 &up = {0.f, 1.f, 0.f});
};
} // namespace sge

#endif // SGE_CCAMERA3_H