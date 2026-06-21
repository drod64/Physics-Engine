#ifndef SGE_FORCE_PARAMS_H
#define SGE_FORCE_PARAMS_H
#include <SM/Vec3.h>

namespace sge {
    // Can be used for both anchored spring and anchored bungee force generation.
    struct AnchorSpring3Params {
        sm::Vec3 anchorPosition;
        sm::real springConstant;
        sm::real restLength;
    };

    struct Buoyancy3Params {
        sm::real maxDepth;
        sm::real volume;
        sm::real waterHeight;
        sm::real liquidDensity;
    };

    struct Drag3Params {
        sm::real k1;
        sm::real k2;
    };

    struct FakeStiffSpring3Params {
        sm::Vec3 anchorPosition;
        sm::real springConstant;
        sm::real damping;
    };

    struct Gravity3Params {
        sm::Vec3 gravity;
    };
}

#endif // SGE_FORCE_PARAMS_H