#include <SGE/core/ecs/components/CLifespan.h>

namespace sge {

CLifespan::CLifespan() :
CLifespan(0)
{}

CLifespan::CLifespan(int total) :
total(total),
remaining(total)
{}

} // namespace sge