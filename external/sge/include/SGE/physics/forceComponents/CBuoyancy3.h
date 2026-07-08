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

    /**
     * Parameterized constructor.
     * @param maxDepth the max y-level depth
     * @param volume the volume of the liquid
     * @param waterHeight the height from the depth
     * @param liquidDensity the liquid's density (defaults to 100.f)
     */
    CBuoyancy3(sm::real maxDepth, sm::real volume, sm::real waterHeight, sm::real liquidDensity = 100.0f)
    {
        this->maxDepth = maxDepth;
        this->volume = volume;
        this->waterHeight = waterHeight;
        this->liquidDensity = liquidDensity;
    }
}; // class CBuoyancy3
} // namespace sge

#endif // CBUOYANCY3_H