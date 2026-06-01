#ifndef SPE_PARTICLE3_GRAVITY_FORCE_H
#define SPE_PARTICLE3_GRAVITY_FORCE_H
#include <SM/Vec3.h>
#include <SPE/Particle3.h>
#include <SPE/Particle3ForceGenerator.h>

namespace spe {
class Particle3GravityForce : public Particle3ForceGenerator {
private:
    sm::Vec3 m_gravity;

public:
    Particle3GravityForce(const sm::Vec3 &gravity);

    void updateForce(Particle3 *particle, sm::real dt) override;
};
}

#endif // SPE_PARTICLE3_GRAVITY_FORCE_H