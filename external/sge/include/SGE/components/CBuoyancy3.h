#ifndef CBUOYANCY3_H
#define CBUOYANCY3_H
#include <SM/Precision.h>

namespace sge {
class CBuoyancy3 {
public:
    sm::real maxDepth;
    sm::real volume;
    sm::real waterHeight;
    sm::real liquidDensity;

    CBuoyancy3(sm::real maxDepth, sm::real volume, sm::real waterHeight, sm::real liquidDensity = 100.0f);
};
}

#endif // CBUOYANCY3_H