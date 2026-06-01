#ifndef SPE_PARTICLE3_FORCE_REGISTRY_H
#define SPE_PARTICLE3_FORCE_REGISTRY_H
#include <vector>
#include <algorithm>
#include <SM/Precision.h>
#include <SPE/Particle3.h>
#include <SPE/Particle3ForceGenerator.h>

namespace spe {
class Particle3ForceRegistry {
protected:
    /**
     * Keeps track of particle and force generator.
     */
    struct Particle3ForceRegistration {
        Particle3 *particle;
        Particle3ForceGenerator *fg;
    };

    typedef std::vector<Particle3ForceRegistration> Registry;
    Registry m_registrations;                         // member variable

public:
    Particle3ForceRegistry();

    void add(Particle3 *particle, Particle3ForceGenerator *fg);
    void remove(Particle3 *particle, Particle3ForceGenerator *fg);

    void clear();

    void updateForces(sm::real dt);
};
}

#endif // SPE_PARTICLE3_FORCE_REGISTRY_H