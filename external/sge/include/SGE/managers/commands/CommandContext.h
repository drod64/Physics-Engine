#ifndef SGE_COMMAND_CONTEXT_H
#define SGE_COMMAND_CONTEXT_H
#include <SGE/managers/Registry.h>
#include <exception>

namespace sge {
class CommandContext {
private:
    Registry &m_registry;
    const std::vector<Entity> &m_translationTable;

public:
    /**
     * Parameterized constructor.
     * @param registry a reference to the active registry
     * @param translationTable a reference to the entity translation table
     */
    CommandContext(Registry &registry, const std::vector<Entity> &translationTable) :
    m_registry(registry),
    m_translationTable(translationTable)
    {
    }

    /**
     * @return the active registry
     */
    Registry& getRegistry()
    {
        return this->m_registry;
    }

    /**
     * Maps a buffer Entity ID to a real Entity ID.
     * @param bufferID a sge::Entity ID returned by a CommandBuffer
     * @return the real sge::Entity ID or sge::Entity::INVALID if the bufferID was invalid
     */
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