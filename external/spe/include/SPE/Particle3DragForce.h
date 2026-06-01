#ifndef SPE_PARTICLE3_DRAG_FORCE_H
#define SPE_PARTICLE3_DRAG_FORCE_H
#include <SM/Vec3.h>
#include <SPE/Particle3.h>
#include <SPE/Particle3ForceGenerator.h>

namespace spe {
class Particle3DragForce : public Particle3ForceGenerator {
private:
    sm::real k1;
    sm::real k2;

public:
    Particle3DragForce(sm::real k1, sm::real k2);

    void updateForce(Particle3 *particle, sm::real dt) override;
};
}

#endif // SPE_PARTICLE3_DRAG_FORCE_H