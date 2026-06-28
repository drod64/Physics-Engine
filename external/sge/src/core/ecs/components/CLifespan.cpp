#include <SGE/core/ecs/components/CLifespan.h>

namespace sge {

CLifespan::CLifespan(sm::real total) :
total(total),
remaining(total)
{}

} // namespace sge