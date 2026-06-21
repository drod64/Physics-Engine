#ifndef SGE_FORCE_EFFECTS_H
#define SGE_FORCE_EFFECTS_H
#include <vector>
#include <SGE/components/physics/CForce.h>

namespace sge {
    /**
     * Struct that holds a collection of active non-jointed forces.
     */
    struct CForceEffects {
        std::vector<CForce> forces;
    };
}

#endif // SGE_FORCE_EFFECTS_H