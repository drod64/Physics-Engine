#ifndef SGE_CTRANSFORM3_H
#define SGE_CTRANSFORM3_H
#include <sm/Vec3.h>
#include <SGE/components/Component.h>

namespace sge {
class CTransform3 : public Component {
public:
    sm::Vec3 position;
    sm::Vec3 rotation;

    CTransform3();
    
};
} // namespace sge

#endif // SGE_CTRANSFORM3_H