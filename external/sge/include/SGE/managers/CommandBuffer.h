#ifndef SGE_COMMAND_BUFFER_H
#define SGE_COMMAND_BUFFER_H
#include <algorithm>
#include <vector>
#include <memory>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/Registry.h>
#include <SGE/util/ByteStream.h>
#include <iostream>

namespace sge {
// Forward declaration of Registry class. 
class Registry;

class CommandBuffer {
private:
    using DeferredCommandFunction = void(*)(sge::Registry &, sge::Entity, size_t, sge::ByteStream &);

    enum class CommandType {
        AddComponentDeferred,
        RemoveComponentDeferred
    };

    template <typename T>
    static void componentRegistrationDispatcher(sge::Registry &registry, sge::Entity e, size_t handleIndex, sge::ByteStream &byteStream)
    {
        std::cout << "[CommandBuffer]: In handler!\n";
        // 4. Read payload.
        const T &componentPayload = byteStream.getByHandle<T>(handleIndex);

        // Update byte stream to next block.
        byteStream.setReadPosition(handleIndex + sizeof(T));
        std::cout << "[CommandBuffer]: Read payload.\n";

        if (e != sge::Entity::INVALID)
        {
            registry.addComponent(e, componentPayload);
        }
        std::cout << "[CommandBuffer]: Component added to registry.\n";
    }

    // Byte stream of data.
    ByteStream m_byteStream;

    // Fake ID counter.
    uint32_t m_placeHolderIndex;

    // Pending commands.
    std::vector<Entity> m_pendingEntityDestructions;

    //Fake ID ---> Real ID
    std::vector<Entity> m_entityTranslationTable; // no reset to save on allocation calls

    void flushByteStream(Registry &registry);

public:
    explicit CommandBuffer(size_t initialCapacity = 1024);

    // Prevent copying.
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator= (const CommandBuffer&) = delete;

    // Allow moving the buffer between systems or threads.
    CommandBuffer(CommandBuffer&&) noexcept = default;
    CommandBuffer& operator= (CommandBuffer&&) noexcept = default;

    Entity createEntityDeferred();

    void destroyEntityDeferred(Entity e);

    template <typename T>
    CommandBuffer& addComponentDeferred(Entity e, const T &component);

    template <typename T>
    CommandBuffer& removeComponentDeferred(Entity e);

    // This is the spot where real Entities are spawned in the Registry (usually at the start of a new frame). 
    void playBack(Registry &registry);

    void clear();
};
} // namespace sge

// Implementation

inline void sge::CommandBuffer::flushByteStream(sge::Registry &registry)
{
    while (this->m_byteStream.getReadPosition() < this->m_byteStream.size())
    {
        // 1. Read command type.
        CommandType type = this->m_byteStream.read<CommandType>();
        std::cout << "[CommandBuffer]: Read CommandType.\n";

        switch (type)
        {
            case CommandType::AddComponentDeferred: {
                // 2. Read handler.
                auto handler = this->m_byteStream.read<DeferredCommandFunction>();

                // 3. Read Entity ID
                sge::Entity targetEntity = this->m_byteStream.read<sge::Entity>();

                std::cout << "[CommandBuffer]: Read handler and target entity.\n";

                if (sge::IsFakeEntity(targetEntity))
                {
                    uint32_t fakeID = static_cast<uint32_t>(targetEntity);
                    uint32_t tableIndex = fakeID & sge::REAL_ENTITY_MASK;
                    targetEntity = this->m_entityTranslationTable[tableIndex];
                }

                if (targetEntity == sge::Entity::INVALID)
                {
                    break;
                }

                size_t payloadHandle = this->m_byteStream.getReadPosition();
                std::cout << "[CommandBuffer]: Invoking handler.\n";
                handler(registry, targetEntity, payloadHandle, this->m_byteStream);
                break;
            }

            case CommandType::RemoveComponentDeferred:
                // TODO
                break;
            
            default:
                break;
        }
    }
}

inline sge::CommandBuffer::CommandBuffer(size_t initialCapacity) :
m_byteStream(initialCapacity)
{
    this->m_placeHolderIndex = 0;
    this->m_entityTranslationTable.reserve(64);
    this->m_pendingEntityDestructions.reserve(64);
}

inline sge::Entity sge::CommandBuffer::createEntityDeferred()
{
    uint32_t fakeID = this->m_placeHolderIndex | sge::FAKE_ENTITY_FLAG;
    
    ++this->m_placeHolderIndex;
    
    return static_cast<Entity>(fakeID);
}

inline void sge::CommandBuffer::destroyEntityDeferred(Entity e)
{   
    // Only destroy entities that are invalid. 
    if (e == sge::Entity::INVALID) return;

    // Fake entities are allowed in order to support same frame creation and deletion.
    // These are intercepted in playBack().
    this->m_pendingEntityDestructions.push_back(e);
}

template<typename T>
inline sge::CommandBuffer& sge::CommandBuffer::addComponentDeferred(sge::Entity e, const T &component)
{
    using CleanType = std::decay_t<T>;

    // 1. Write command type.
    this->m_byteStream.write<CommandType>(CommandType::AddComponentDeferred);

    std::cout << "[CommandBuffer]: Wrote CommandType.\n";
    
    // 2. Write handler functor.
    auto handler = &sge::CommandBuffer::componentRegistrationDispatcher<CleanType>;
    this->m_byteStream.write<DeferredCommandFunction>(handler);

    // 3. Write Entity ID.
    this->m_byteStream.write<sge::Entity>(e);

    // 4. Write component payload
    this->m_byteStream.write<CleanType>(component);
    
    std::cout << "[CommandBuffer]: Wrote functor, ID, and payload.\n";

    return *this;
}

template<typename T>
inline sge::CommandBuffer& sge::CommandBuffer::removeComponentDeferred(sge::Entity e)
{
    return *this;
}

inline void sge::CommandBuffer::playBack(sge::Registry &registry)
{
    // Sort and deduplicate pending destructions vector.
    std::sort(this->m_pendingEntityDestructions.begin(), this->m_pendingEntityDestructions.end());
    auto last = std::unique(this->m_pendingEntityDestructions.begin(), this->m_pendingEntityDestructions.end());
    this->m_pendingEntityDestructions.erase(last, this->m_pendingEntityDestructions.end());

    // Grow translation table if created entities spikes.
    if (this->m_placeHolderIndex > this->m_entityTranslationTable.size())
    {
        this->m_entityTranslationTable.resize(this->m_placeHolderIndex);
    }

    // 1. Map out real IDs for Entities.
    for (uint32_t i = 0; i < this->m_placeHolderIndex; ++i)
    {
        uint32_t fakeID = i | sge::FAKE_ENTITY_FLAG;
        sge::Entity fakeHandle = static_cast<sge::Entity>(fakeID);
        uint32_t tableIndex = fakeID & sge::REAL_ENTITY_MASK;

        // Ignore fake entities that are pending destruction.
        // This saves us from unnecessary creation because of immediate deletion.
        if (std::binary_search(this->m_pendingEntityDestructions.begin(), this->m_pendingEntityDestructions.end(), fakeHandle))
        {
            this->m_entityTranslationTable[tableIndex] = sge::Entity::INVALID;
            continue;
        }

        // Request a permanent ID for the Entity.
        sge::Entity realID = registry.createEntity();
        
        // Map temp fake ID to permanent ID.
        this->m_entityTranslationTable[tableIndex] = realID;
    }
    
    std::cout << "[CommandBuffer]: Begin stream flush.\n";
    // 2. Process commands in byte stream.
    this->flushByteStream(registry);
    std::cout << "[CommandBuffer]: Finished stream flush.\n";
    
    // 3. Process destructions.
    for (const Entity &e : this->m_pendingEntityDestructions)
    {
        // Skip fake entities. These were intercepted and...
        //...never created in the registry.
        if (sge::IsFakeEntity(e))
        {
            continue;
        }
        
        // Final check. See if Entity is still alive before destroying.
        if (registry.isAlive(e))
        {
            registry.destroyEntity(e);
        }
    }

    this->clear();
}

inline void sge::CommandBuffer::clear()
{
    this->m_byteStream.clear();
    this->m_pendingEntityDestructions.clear();
    this->m_placeHolderIndex = 0;
}

#endif // SGE_COMMAND_BUFFER_H