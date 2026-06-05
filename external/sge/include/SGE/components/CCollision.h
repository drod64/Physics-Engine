#ifndef SGE_CCOLLISION_H
#define SGE_CCOLLISION_H
#include <SGE/components/Component.h>

namespace sge {
/**
 * Component that stores collision data (pending...)
 */
class CCollision : public Component {
public:
    float radius;

    /**
     * Default Constructor.
     */
    CCollision();

    /**
     * Parameterized Constructor.
     */
    CCollision(float radius);
};

} // namespace sge

#endif // SGE_CCOLLISION_H