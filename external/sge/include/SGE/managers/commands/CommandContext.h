#ifndef SGE_COMMAND_CONTEXT_H
#define SGE_COMMAND_CONTEXT_H
#include <SGE/managers/Registry.h>
#include <exception>
#include <iostream>

namespace sge {
class CommandContext {
private:
    Registry &m_registry;
    const std::vector<Entity> &m_translationTable;

public:
    CommandContext(Registry &registry, const std::vector<Entity> &translationTable) :
    m_registry(registry),
    m_translationTable(translationTable)
    {
    }

    Registry& getRegistry()
    {
        return this->m_registry;
    }

    Entity getEntity(Entity bufferID) const
    {
        if (sge::IsFakeEntity(bufferID))
        {
            uint32_t fakeID = static_cast<uint32_t>(bufferID);
            uint32_t tableIndex = fakeID & sge::REAL_ENTITY_MASK;

            if (tableIndex < this->m_translationTable.size())
            {
                return this->m_translationTable[tableIndex];
            }
        }

        return sge::Entity::INVALID;
    }
}; // class CommandContext
} // namespace sge

#endif // SGE_COMMAND_CONTEXT_H