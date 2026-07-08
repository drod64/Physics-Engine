#ifndef SGE_CGRAVITY3_H
#define SGE_CGRAVITY3_H
#include <SM/Vec3.h>

namespace sge {
class CGravity3 {
public:
    sm::Vec3 gravity;

    /**
     * Explicit constructor that accepts the gravity force that should be applied.
     * @param gravity the desired gravity force
     */
    CGravity3(const sm::Vec3 &gravity)
    {
        this->gravity = gravity;
    }
}; // class CGravity3
} // namespace sge

#endif // SGE_CGRAVITY3_H