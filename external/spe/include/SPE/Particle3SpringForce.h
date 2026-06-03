#ifndef SPE_PARTICLE3_SPRING_FORCE_H
#define SPE_PARTICLE3_SPRING_FORCE_H
#include <SM/Precision.h>
#include <SPE/Particle3.h>
#include <SPE/Particle3ForceGenerator.h>

namespace spe {
class Particle3SpringForce : public Particle3ForceGenerator {
private:
    Particle3 *m_other;
    sm::real m_springConstant;
    sm::real m_restLength;

public:
    Particle3SpringForce(Particle3 *other, sm::real springConstant, sm::real restLength);

    void updateForce(Particle3 *particle, sm::real dt) override;
};
}

#endif // SPE_PARTICLE3_SPRING_FORCE_H