#include <SGE/components/CBuoyancy3.h>

sge::CBuoyancy3::CBuoyancy3(sm::real maxDepth, sm::real volume, sm::real waterHeight, sm::real liquidDensity)
{
    this->maxDepth = maxDepth;
    this->volume = volume;
    this->waterHeight = waterHeight;
    this->liquidDensity = liquidDensity;
}