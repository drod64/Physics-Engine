#include <SGE/core/ecs/components/CLifespan.h>

namespace sge {

CLifespan::CLifespan() :
CLifespan(0)
{}

CLifespan::CLifespan(sm::real total) :
total(total),
remaining(total)
{}

} // namespace sge