#ifndef SGE_SPRING_FORCE3_H
#define SGE_SPRING_FORCE3_H
#include <SM/Precision.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class SpringForce3 : public ForceGenerator3 {
private:
    Entity *m_other;
    sm::real m_springConstant;
    sm::real m_restLength;

public:
    SpringForce3(Entity *other, sm::real springConstant, sm::real restLength);

    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_SPRING_FORCE3_H