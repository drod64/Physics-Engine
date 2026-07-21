#ifndef SGE_BUNGEE_CONSTRAINT_COMMAND_H
#define SGE_BUNGEE_CONSTRAINT_COMMAND_H
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/commands/CommandContext.h>
#include <SGE/physics/constraints/types/BungeeConstraint.h>

namespace sge {
struct BungeeConstraintCommand {
    Entity entityA;
    Entity entityB;
    sm::real springConstant;
    sm::real restLength;
    sm::Vec3 localAttachPoint;

    /**
     * Executes constraint logic for a bungee spring connection.
     * @param context the sge::CommandContext containing the active registry and translation table
     */
    void execute(CommandContext &context) const
    {
        Entity realA = context.getEntity(this->entityA);
        Entity realB = context.getEntity(this->entityB);

        BungeeConstraint bungeePayload {
            .entityA = realA,
            .entityB = realB,
            .restLength = this->restLength,
            .springConstant = this->springConstant,
            .localAttachPoint = this->localAttachPoint
        };

        context.getRegistry().addConstraint(bungeePayload);
    }
}; // struct BungeeConstraintCommand
} // namespace sge

#endif // SGE_BUNGEE_CONSTRAINT_COMMAND_H