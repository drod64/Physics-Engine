#ifndef SGE_SPRING_CONSTRAINT_COMMAND_H
#define SGE_SPRING_CONSTRAINT_COMMAND_H
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/commands/CommandContext.h>
#include <SGE/physics/constraints/types/SpringConstraint.h>

namespace sge {
struct SpringConstraintCommand {
    Entity entityA = sge::Entity::INVALID;
    Entity entityB = sge::Entity::INVALID;
    sm::real springConstant = 10;
    sm::real restLength = 10;
    sm::Vec3 localAttachPoint;

    /**
     * Executes constraint logic for a regular spring connection.
     * @param context the sge::CommandContext containing the active registry and translation table
     */
    void execute(CommandContext &context) const
    {
        Entity realA = context.getEntity(this->entityA);
        Entity realB = context.getEntity(this->entityB);

        if (realA == Entity::INVALID || realB == Entity::INVALID)
        {
            return;
        }

        SpringConstraint constraintPayload {
            .entityA = realA,
            .entityB = realB,
            .springConstant = this->springConstant,
            .restLength = this->restLength,
            .localAttachPoint = this->localAttachPoint
        };

        context.getRegistry().addConstraint(constraintPayload);
    }

}; // struct SpringConstraintCommand
} // namespace sge

#endif // SGE_SPRING_CONSTRAINT_COMMAND_H