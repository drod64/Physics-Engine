#ifndef SGE_GRAVITY_FORCE3_H
#define SGE_GRAVITY_FORCE3_H
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class GravityForce3 :public ForceGenerator3 {
private:
    sm::Vec3 m_gravity;

public:
    GravityForce3(const sm::Vec3 &gravity);

    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_GRAVITY_FORCE3_H