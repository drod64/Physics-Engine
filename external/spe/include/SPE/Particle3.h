#ifndef SPE_PARTICLE_H
#define SPE_PARTICLE_H
#include <SM/Vec3.h>
#include <assert.h>

namespace spe {

class Particle3 {
public:
    sm::Vec3 position;
    sm::Vec3 velocity;
    sm::Vec3 acceleration;
    sm::real damping;

    Particle3();

    Particle3(const sm::Vec3 &pos, const sm::Vec3 &vel, const sm::Vec3 &acc, sm::real damping);

    void setInverseMass(sm::real inverseMass);
    void setMass(sm::real mass);
    
    sm::real getInverseMass() const;
    sm::real getMass() const;

    void integrate(sm::real duration);

protected:
    sm::real inverseMass;
    sm::Vec3 forceAccum;

};

} // namespace spe

#endif // SPE_PARTICLE_H