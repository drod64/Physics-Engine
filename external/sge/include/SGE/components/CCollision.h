#ifndef SGE_CCOLLISION_H
#define SGE_CCOLLISION_H
#include <SGE/components/Component.h>

namespace sge {
    
class CCollision : public Component {
public:
    float radius;

    CCollision();
    CCollision(float radius);
};

} // namespace sge

#endif // SGE_CCOLLISION_H