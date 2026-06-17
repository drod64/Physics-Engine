#ifndef SGE_COMMAND_BUFFER_H
#define SGE_COMMAND_BUFFER_H
#include <vector>
#include <unordered_map>
#include <memory>
#include <SGE/entities/Entity.h>
#include <SGE/core/Registry.h>

namespace sge {
// Forward declaration of Registry class. 
class Registry;

class CommandBuffer {
private:
    using CommandExecutorFn = void(*)(Registry& registry, 
                                    const std::unordered_map<uint32_t, sge::Entity>& translationMap, 
                                    const uint8_t* payloadBuffer);
    
    struct CommandHeader {
        CommandExecutorFn executor;
        uint32_t totalCommandSize;
    };

    std::vector<uint8_t> m_rawByteBuffer;
    std::vector<Entity> m_pendingDestructions;
    std::unordered_map<uint32_t, Entity> m_translationMap;
    uint32_t m_placerHolderIndex;

public:
    CommandBuffer() = default;
    ~CommandBuffer() = default;

    // Prevent copying.
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator= (const CommandBuffer&) = delete;

    // Allow moving the buffer between systems or threads.
    CommandBuffer(CommandBuffer&&) noexcept = default;
    CommandBuffer& operator= (CommandBuffer&&) noexcept = default;

    Entity createEntityDeferred();

    void destroyEntityDeferred(Entity e);

    template <typename T>
    CommandBuffer& addComponentDeferred(Entity e, T&& component);

    // This is the spot where real Entities are spawned in the Registry (usually at the start of a new frame). 
    void playBack(Registry &registry);

    void clear();
};
} // namespace sge

// Implementation

inline sge::Entity sge::CommandBuffer::createEntityDeferred()
{
    uint32_t fakeID = this->m_placerHolderIndex | sge::FAKE_ENTITY_FLAG;

    ++this->m_placerHolderIndex;

    return static_cast<Entity>(fakeID);
}

inline void sge::CommandBuffer::destroyEntityDeferred(Entity e)
{
    // Do not destroy entities that do not exist.
    if (sge::IsFakeEntity(e)) return;
    
    if (e == sge::Entity::INVALID) return;

    this->m_pendingDestructions.push_back(e);
}

template<typename T>
inline sge::CommandBuffer& sge::CommandBuffer::addComponentDeferred(sge::Entity e, T&& component)
{
    // Extract the clean underlying value type (strips references and const/volatile modifiers)
    using ComponentType = std::decay_t<T>;

    // 1. Callback function for unpacking the data.
    CommandExecutorFn staticExecutor = [](Registry &registry, 
                                        const std::unordered_map<uint32_t, sge::Entity>& translationMap, 
                                        const uint8_t* payloadBuffer)
    {
        // Unpack component data using the pure value type.
        const ComponentType* packedComponent = reinterpret_cast<const ComponentType*>(payloadBuffer);

        // Get target Entity ID (offset accurately by the component value size).
        sge::Entity targetID = *reinterpret_cast<const sge::Entity*>(payloadBuffer + sizeof(ComponentType));

        // Look for real ID
        if (sge::IsFakeEntity(targetID)) 
        {
            uint32_t fakeKey = static_cast<uint32_t>(targetID);

            auto it = translationMap.find(fakeKey);
            if (it != translationMap.end()) 
            {
                targetID = it->second;
            } 
            else 
            {
                // Fake ID was never processed. Exit early.
                return;
            }
        }

        // Add component to target Entity ID.
        if (registry.isAlive(targetID)) 
        {
            // Explicitly pass the value type to registry.addComponent
            registry.addComponent<ComponentType>(targetID, std::move(*const_cast<ComponentType*>(packedComponent)));
        }
    };

    // 2. Calculating memory sizes using ComponentType
    size_t currentOffSet = this->m_rawByteBuffer.size();
    size_t headerSize = sizeof(CommandHeader);
    size_t payloadSize = sizeof(ComponentType);
    size_t entitySize = sizeof(sge::Entity);
    size_t totalBlockSize = headerSize + payloadSize + entitySize;

    // 3. Expand contiguous byte stream
    this->m_rawByteBuffer.resize(currentOffSet + totalBlockSize);

    // 4. Write meta header
    CommandHeader* headerLocation = reinterpret_cast<CommandHeader*>(&this->m_rawByteBuffer[currentOffSet]);
    headerLocation->executor = staticExecutor;
    headerLocation->totalCommandSize = static_cast<uint32_t>(totalBlockSize);

    // 5. Write the component struct bytes using placement new on ComponentType
    uint8_t* componentPayloadLocation = &this->m_rawByteBuffer[currentOffSet + headerSize];
    new (componentPayloadLocation) ComponentType(std::forward<T>(component));

    // 6. Write the entity target ID
    sge::Entity *entityLocation = reinterpret_cast<sge::Entity*>(componentPayloadLocation + payloadSize);
    *entityLocation = e;

    // Return *this for chaining.
    return *this;
}

inline void sge::CommandBuffer::playBack(sge::Registry &registry)
{
    // 1. Process destructions.
    for (Entity e : this->m_pendingDestructions)
    {
        registry.destroyEntity(e);
    }
    this->m_pendingDestructions.clear();

    // 2. Map out real IDs for Entities.
    for (uint32_t i = 0; i < this->m_placerHolderIndex; ++i)
    {
        uint32_t fakeID = i | sge::FAKE_ENTITY_FLAG;

        // Request a permanent ID for the Entity.
        sge::Entity realID = registry.createEntity();

        // Map temp fake ID to permanent ID.
        this->m_translationMap[fakeID] = realID;
    }
    this->m_placerHolderIndex = 0;

    // 3. Stream through the raw byte commands.
    size_t byteIterator = 0;
    size_t totalBytes = this->m_rawByteBuffer.size();

    while (byteIterator < totalBytes)
    {
        // Read meta header.
        const CommandHeader *header = reinterpret_cast<const CommandHeader*>(&this->m_rawByteBuffer[byteIterator]);

        // Calculate where payload data is.
        const uint8_t *payloadPtr = &this->m_rawByteBuffer[byteIterator + sizeof(CommandHeader)];
        
        // Invoke the type-erased lambda function to add components to live registry.
        header->executor(registry, this->m_translationMap, payloadPtr);

        // Advance to next block.
        byteIterator += header->totalCommandSize;
    }

    this->clear();
}

inline void sge::CommandBuffer::clear()
{
    this->m_rawByteBuffer.clear();
    this->m_pendingDestructions.clear();
    this->m_translationMap.clear();
    this->m_placerHolderIndex = 0;
}

#endif // SGE_COMMAND_BUFFER_H