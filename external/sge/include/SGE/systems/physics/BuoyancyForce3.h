#ifndef SGE_BUOYANCY_FORCE3_H
#define SGE_BUOYANCY_FORCE3_H
#include <SM/Precision.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class BuoyancyForce3 : public ForceGenerator3 {
private:
    sm::real m_maxDepth;
    sm::real m_volume;
    sm::real m_waterHeight;
    sm::real m_liquidDensity;

public:
    BuoyancyForce3(sm::real maxDepth, sm::real volume, sm::real waterHeight, sm::real liquidDensity = 1000.0f);

    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_BUOYANCY_FORCE3_H