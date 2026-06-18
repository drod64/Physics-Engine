#ifndef SGE_COMMAND_BUFFER_H
#define SGE_COMMAND_BUFFER_H
#include <algorithm>
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
        void (*destructor) (uint8_t *payloadBuffer);
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

    template <typename T>
    CommandBuffer& removeComponentDeferred(Entity e);

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

        // Calculate the aligned position where the Entity ID was stored.
        constexpr size_t ALIGN_SIZE = 8;
        size_t payloadSize = sizeof(ComponentType);
        size_t paddingAfterPayload = (ALIGN_SIZE - (payloadSize % ALIGN_SIZE)) % ALIGN_SIZE;
        
        // Get target Entity ID (offset accurately by the component value size).
        sge::Entity targetID = *reinterpret_cast<const sge::Entity*>(payloadBuffer + payloadSize + paddingAfterPayload);
        
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

    // 2. 8 byte Data-Oriented alignment.
    constexpr size_t ALIGN_SIZE = 8;
    size_t headerStart = this->m_rawByteBuffer.size();

    // Calculate aligned space for the header data.
    size_t headerSize = sizeof(CommandHeader);
    size_t paddingAfterHeading = (ALIGN_SIZE - (headerSize % ALIGN_SIZE)) % ALIGN_SIZE;
    size_t payloadStart = headerStart + headerSize + paddingAfterHeading;

    // Calculate aligned space for component data.
    size_t payloadSize = sizeof(ComponentType);
    size_t paddingAfterPayload = (ALIGN_SIZE - (payloadSize % ALIGN_SIZE)) % ALIGN_SIZE;
    size_t entityStart = payloadStart + payloadSize + paddingAfterPayload;

    // Calculate aligned space for Entity ID.
    size_t entitySize = sizeof(sge::Entity);
    size_t paddingAfterEntity = (ALIGN_SIZE - (entitySize % ALIGN_SIZE)) % ALIGN_SIZE;
    size_t nextBlockStart = entityStart + entitySize + paddingAfterEntity;

    // Calculate total footprints.
    size_t totalAllocatedBytes = nextBlockStart - headerStart;
    uint32_t playBackHopSize = static_cast<uint32_t>(nextBlockStart - headerStart);

    // 3. Expand byte buffer.
    this->m_rawByteBuffer.resize(headerStart + totalAllocatedBytes);

    // Grab a safe/stable pointer to potentially new allocated heap block.
    uint8_t *baseBufferPtr = this->m_rawByteBuffer.data();

    // 4. Write meta header data to its aligned block.
    CommandHeader *headerLocation = reinterpret_cast<CommandHeader*>(baseBufferPtr + headerStart);
    headerLocation->executor = staticExecutor;
    headerLocation->totalCommandSize = playBackHopSize;
    headerLocation->destructor = [](uint8_t *payloadBuffer) {
        reinterpret_cast<ComponentType*>(payloadBuffer)->~ComponentType();
    };

    // 5. Write component data bytes using placement new on aligned pointer.
    uint8_t *componentPayloadLocation = baseBufferPtr + payloadStart;
    new (componentPayloadLocation) ComponentType(std::forward<T>(component));

    // 6. Write Entity ID to its aligned block.
    uint8_t *entityPayloadLocation = baseBufferPtr + entityStart;
    sge::Entity *entityLocation = reinterpret_cast<sge::Entity*>(entityPayloadLocation);
    *entityLocation = e;
    
    return *this;
}

template<typename T>
inline sge::CommandBuffer& sge::CommandBuffer::removeComponentDeferred(sge::Entity e)
{
    // Extract the clean underlying value type.
    using ComponentType = std::remove_cv_t<std::remove_reference_t<T>>;

    // 1. Callback function for unpacking the data.
    CommandExecutorFn staticExecutor = [](Registry &registry, 
                                        const std::unordered_map<uint32_t, sge::Entity>& translationMap, 
                                        const uint8_t* payloadBuffer)
    {
        // Get target Entity ID (front of payload buffer).
        sge::Entity targetID = *reinterpret_cast<const sge::Entity*>(payloadBuffer);

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

        // Remove component from target entity ID.
        if (registry.isAlive(targetID) && registry.hasComponent<ComponentType>(targetID)) 
        {
            // Explicitly pass the value type to registry to remove.
            registry.removeComponent<ComponentType>(targetID);
        }
    };

    // 2. 8 byte Data-Oriented alignment
    constexpr size_t ALIGN_SIZE = 8;

    size_t startOffset = this->m_rawByteBuffer.size();

    // Align start position of new block.
    size_t paddingBeforeHeader = (ALIGN_SIZE - (startOffset % ALIGN_SIZE)) % ALIGN_SIZE;
    size_t headerStart = startOffset + paddingBeforeHeader;

    // Calculate aligned space for the header.
    size_t headerSize = sizeof(CommandHeader);
    size_t paddingAfterHeader = (ALIGN_SIZE - (headerSize % ALIGN_SIZE)) % ALIGN_SIZE;
    size_t payloadStart = headerStart + headerSize + paddingAfterHeader;

    // Calculate aligned space for the entity payload data.
    size_t entitySize = sizeof(sge::Entity);
    size_t paddingAfterEntity = (ALIGN_SIZE - (entitySize % ALIGN_SIZE)) % ALIGN_SIZE;
    size_t nextBlockStart = payloadStart + entitySize + paddingAfterEntity;

    // Calculate total footprint from initial allocation boundary.
    size_t totalAllocatedBytes = nextBlockStart - startOffset;

    // Distance playBack() must traverse to reach the next header slot.
    uint32_t playBackHopSize = static_cast<uint32_t>(nextBlockStart - headerStart);

    // 3. Expand byte buffer.
    this->m_rawByteBuffer.resize(startOffset + totalAllocatedBytes);

    // Grab a stable/safe pointer to potential new heap block.
    uint8_t *baseBufferPtr = this->m_rawByteBuffer.data();

    // 4. Write meta header to its aligned layout block.
    CommandHeader *headerLocation = reinterpret_cast<CommandHeader*>(baseBufferPtr + headerStart);
    headerLocation->executor = staticExecutor;
    headerLocation->totalCommandSize = playBackHopSize;
    headerLocation->destructor = nullptr;

    // 5. Write Entity ID payload to its block.
    uint8_t *entityPayloadLocation = baseBufferPtr + payloadStart;
    sge::Entity *entityLocation = reinterpret_cast<sge::Entity*>(entityPayloadLocation);
    *entityLocation = e;

    return *this;
}

inline void sge::CommandBuffer::playBack(sge::Registry &registry)
{
    // 1. Map out real IDs for Entities.
    for (uint32_t i = 0; i < this->m_placerHolderIndex; ++i)
    {
        uint32_t fakeID = i | sge::FAKE_ENTITY_FLAG;

        // Request a permanent ID for the Entity.
        sge::Entity realID = registry.createEntity();
        
        // Map temp fake ID to permanent ID.
        this->m_translationMap[fakeID] = realID;
    }
    this->m_placerHolderIndex = 0;

    // 2. Stream through the raw byte commands.
    // This mutates their components (adding or removing).
    size_t byteIterator = 0;
    size_t totalBytes = this->m_rawByteBuffer.size();
    
    if (totalBytes > 0)
    {
        const uint8_t *baseBufferPtr = this->m_rawByteBuffer.data();

        while (byteIterator < totalBytes)
        {
            constexpr size_t ALIGN_SIZE = 8;
            size_t headerSize = sizeof(CommandHeader);
            size_t paddingAfterHeader = (ALIGN_SIZE - (headerSize % ALIGN_SIZE)) % ALIGN_SIZE;

            // Read meta header.
            const CommandHeader *header = reinterpret_cast<const CommandHeader*>(baseBufferPtr + byteIterator);
    
            // Calculate where payload data is (taking into account the padding).
            const uint8_t *payloadPtr = baseBufferPtr + byteIterator + headerSize + paddingAfterHeader;
            
            // Invoke the type-erased lambda function to add/remove components from live registry.
            header->executor(registry, this->m_translationMap, payloadPtr);
    
            // Advance to next block.
            byteIterator += header->totalCommandSize;
        }
    }
    
    // Before destruction phase...
    // ...perform a deduplication on the soon to be destroyed IDs.
    // (rare cases where an entity is created and destroyed in the same frame)
    std::sort(this->m_pendingDestructions.begin(), this->m_pendingDestructions.end());
    auto last = std::unique(this->m_pendingDestructions.begin(), this->m_pendingDestructions.end());
    this->m_pendingDestructions.erase(last, this->m_pendingDestructions.end());

    // 3. Process destructions.
    for (const Entity &e : this->m_pendingDestructions)
    {
        Entity targetID = e;

        // Find real ID if a fake ID managed to sneak it's way into the destruction queue.
        if (sge::IsFakeEntity(targetID))
        {
            auto it = this->m_translationMap.find(static_cast<uint32_t>(targetID));
            if (it != this->m_translationMap.end())
            {
                targetID = it->second;
            }
            else
            {
                continue;
            }
        }
        
        // Final check. See if Entity is still alive before destroying.
        if (registry.isAlive(targetID))
        {
            registry.destroyEntity(targetID);
        }
    }
    this->m_pendingDestructions.clear();

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