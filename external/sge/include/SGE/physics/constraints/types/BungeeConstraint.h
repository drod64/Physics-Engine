#ifndef SGE_BUNGEE_CONSTRAINT_H
#define SGE_BUNGEE_CONSTRAINT_H
#include <SGE/core/ecs/Entity.h>
#include <SM/Precision.h>

namespace sge {
class BungeeConstraint {
public:
    sge::Entity entityA = sge::Entity::INVALID;
    sge::Entity entityB = sge::Entity::INVALID;
    sm::real restLength = 10;
    sm::real springConstant = 10;

}; // class BungeeConstraint
} // namespace sge

#endif // SGE_BUNGEE_CONSTRAINT_H