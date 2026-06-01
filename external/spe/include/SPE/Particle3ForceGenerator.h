#ifndef SPE_PARTICLE3_FORCE_GENERATOR_H
#define SPE_PARTICLE3_FORCE_GENERATOR_H
#include <SM/Precision.h>
#include <SPE/Particle3.h>

namespace spe {

/**
 * Base class for force generators
 */
class Particle3ForceGenerator {
public:

    virtual void updateForce(Particle3 *particle, sm::real dt) = 0;
};
}

#endif // SPE_PARTICLE_FORCE_GENERATOR_H