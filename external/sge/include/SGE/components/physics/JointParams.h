#ifndef SGE_JOINT_PARAMS_H
#define SGE_JOINT_PARAMS_H
#include <SM/Precision.h>

namespace sge {
    struct BungeeParams {
        sm::real springConstant;
        sm::real restLength;
    };

    struct CableParams {
        sm::real maxLength;
        sm::real restitution;
    };

    struct RodParams {
        sm::real length;
    };

    struct SpringParams {
        sm::real springConstant;
        sm::real restLength;
        sm::real damping;
    };
}

#endif // SGE_JOINT_PARAMS_H