#ifndef SGE_DRAG_FORCE3_H
#define SGE_DRAG_FORCE3_H
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class DragForce3 : public ForceGenerator3 {
private:
    sm::real m_k1;
    sm::real m_k2;

public:
    DragForce3(sm::real k1, sm::real k2);

    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_DRAG_FORCE3_H