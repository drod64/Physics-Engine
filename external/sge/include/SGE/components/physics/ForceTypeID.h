#ifndef SGE_FORCE_TYPE_ID_H
#define SGE_FORCE_TYPE_ID_H
#include <cstdint>

namespace sge {
    /**
     * Unique IDs for core physic forces.
     */
    enum class ForceTypeID : uint16_t {
        AnchorBungee,
        AnchorSpring,
        Buoyancy,
        Drag,
        FakeStiffSpring,
        Gravity
    };
}

#endif // SGE_FORCE_TYPE_ID_H