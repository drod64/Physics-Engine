#ifndef SGE_COMMAND_BUFFER_H
#define SGE_COMMAND_BUFFER_H
#include <algorithm>
#include <vector>
#include <memory>
#include <SGE/managers/commands/CommandContext.h>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/Registry.h>
#include <SGE/util/ByteStream.h>

namespace sge {
// Forward declaration of Registry class. 
class Registry;


// Ensure custom command structs implement execute() callback function.
template<typename T>
concept IsCustomCommand = requires(T cmd, CommandContext& ctx) {
    { cmd.execute(ctx) } -> std::same_as<void>;
};

/**
 * Manager class.
 * This class has the responsibility of deferring commands until it's playback() function is called.
 */
class CommandBuffer {
    private:
    using DeferredCommandFunction = void(*)(sge::Registry &, sge::Entity, size_t, const sge::ByteStream &);
    using CustomCommandFunction = void(*)(sge::Registry &, const std::vector<sge::Entity>&, const void*);
    
    enum class CommandType {
        AddComponentDeferred,
        RemoveComponentDeferred,
        CustomCommandDeferred
    };
    
    /**
     * Static helper function for adding components to the registry.
     * @param registry the active registry to add the component to
     * @param e the Entity ID to add the component to
     * @param handleIndex the handle index of the actual component payload
     * @param byteStream the byte stream to read from
     */
    template <typename T>
    static void componentRegistrationDispatcher(sge::Registry &registry, sge::Entity e, size_t handleIndex, const sge::ByteStream &byteStream)
    {
        // 5. Read payload data.
        if (e != sge::Entity::INVALID)
        {
            const unsigned char *baseAddress = reinterpret_cast<const unsigned char*>(byteStream.data());
            
            const unsigned char *payloadAddress = baseAddress + handleIndex;
            
            const T &componentInstance = *reinterpret_cast<const T*>(payloadAddress);
            
            registry.addComponent(e, componentInstance);
        }
    }
    
    /**
     * Static helper function for removing components from Entities.
     * @param registry the active registry to remove the component from
     * @param e the sge::Entity ID to remove the component from
     */
    template <typename T>
    static void componentRemovalDispatcher(sge::Registry &registry, sge::Entity e, size_t, const sge::ByteStream &)
    {
        if (e == sge::Entity::INVALID) return;
        
        // Remove component only if entity exists and has the component.
        if (registry.isAlive(e) && registry.hasComponent<T>(e))
        {
            registry.removeComponent<T>(e);
        }
    }
    
    // Byte stream of data.
    ByteStream m_byteStream;
    
    // Fake ID counter.
    uint32_t m_placeHolderIndex;
    
    // Pending commands.
    std::vector<Entity> m_pendingEntityDestructions;
    
    //Fake ID ---> Real ID
    std::vector<Entity> m_entityTranslationTable; // no reset to save on allocation calls
    
    /**
     * Flushes the byte stream of it's deferred commands.
     * @param registry the active registry needed to fulfill the commands
     */
    void flushByteStream(Registry &registry);
    
public:

    /**
     * Parameterized constructor.
     * @param initialCapacity the initial capacity (in bytes) the CommandBuffer will work with.
     */
    explicit CommandBuffer(size_t initialCapacity = 1024);

    // Prevent copying.
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator= (const CommandBuffer&) = delete;

    // Allow moving the buffer between systems or threads.
    CommandBuffer(CommandBuffer&&) noexcept = default;
    CommandBuffer& operator= (CommandBuffer&&) noexcept = default;

    /**
     * Queues an Entity to be created in the next sync point.
     * 
     * NOTE: this function returns a fake sge::Entity ID that can still
     * be used to add components in the same frame. DO NOT CACHE THIS ID.
     * @return a fake sge::Entity placeholder ID
     */
    Entity createEntityDeferred();

    /**
     * Queues an Entity to be destroyed in the next sync point.
     * 
     * NOTE: This function accepts both placeholder and real IDs.
     * @param e the sge::Entity ID to destroy
     */
    void destroyEntityDeferred(Entity e);

    /**
     * Adds a component type to the requested sge::Entity ID.
     * 
     * NOTE: This function accepts both placeholder and real IDs.
     * @tparam T the component type
     * @param e the sge::Entity ID to add the component to
     * @param component the component to add
     * @return a reference to this
     */
    template <typename T>
    CommandBuffer& addComponentDeferred(Entity e, const T &component);

    /**
     * Removes a component type from the requested sge::Entity ID.
     * 
     * NOTE: This function accepts both placeholder and real IDs.
     * @param e the sge::Entity ID to remove the component from
     * @return a reference to this
     */
    template <typename T>
    CommandBuffer& removeComponentDeferred(Entity e);
    
    template <typename TCommand>
    CommandBuffer& pushCustomCommand(const TCommand &command);

    /**
     * This flushes the CommandBuffer's deferred commands.
     * Call this function during your sync points.
     * @param registry the active registry needed to fulfill the commands
     */
    void playBack(Registry &registry);

    /**
     * Clears all the CommandBuffer's queued commands.
     */
    void clear();
}; // class CommandBuffer
} // namespace sge

// Implementation

inline void sge::CommandBuffer::flushByteStream(sge::Registry &registry)
{
    while (this->m_byteStream.getReadPosition() < this->m_byteStream.size())
    {
        // 1. Read command type.
        CommandType type = this->m_byteStream.read<CommandType>();

        switch (type)
        {
            case CommandType::AddComponentDeferred: {
                // 2. Read handler.
                auto handler = this->m_byteStream.read<DeferredCommandFunction>();

                // 3. Read Entity ID
                sge::Entity targetEntity = this->m_byteStream.read<sge::Entity>();

                // 4. Read payload size
                uint32_t payloadSize = this->m_byteStream.read<uint32_t>();

                size_t payloadHandle = this->m_byteStream.getReadPosition();
                size_t nextCommandHandle = payloadHandle + payloadSize;

                if (sge::IsFakeEntity(targetEntity))
                {
                    uint32_t fakeID = static_cast<uint32_t>(targetEntity);
                    uint32_t tableIndex = fakeID & sge::REAL_ENTITY_MASK;
                    targetEntity = this->m_entityTranslationTable[tableIndex];
                }

                if (targetEntity != sge::Entity::INVALID)
                {
                    handler(registry, targetEntity, payloadHandle, this->m_byteStream);
                }

                this->m_byteStream.setReadPosition(nextCommandHandle);
                break;
            }

            case CommandType::RemoveComponentDeferred: {
                // 2. Read handler.
                auto handler = this->m_byteStream.read<DeferredCommandFunction>();

                // 3. Read Entity ID
                sge::Entity targetEntity = this->m_byteStream.read<sge::Entity>();

                if (targetEntity != sge::Entity::INVALID)
                {
                    handler(registry, targetEntity, 0, this->m_byteStream);
                }
                break;
            }

            case CommandType::CustomCommandDeferred: {
                // 2. Read handler.
                CustomCommandFunction handler = this->m_byteStream.read<CustomCommandFunction>();

                // 3. Read payload size.
                uint32_t payloadSize = this->m_byteStream.read<uint32_t>();
                size_t payloadHandle = this->m_byteStream.getReadPosition();
                size_t nextCommandHandle = payloadHandle + payloadSize;

                // 4. Read payload data.
                const unsigned char *baseBufferAddress = reinterpret_cast<unsigned char*>(this->m_byteStream.data());
                const void *rawData = baseBufferAddress + payloadHandle;

                handler(registry, this->m_entityTranslationTable, rawData);

                // Advance byte stream
                this->m_byteStream.setReadPosition(nextCommandHandle);
                break;
            }
            
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
    
    // 2. Write handler functor.
    auto handler = &sge::CommandBuffer::componentRegistrationDispatcher<CleanType>;
    this->m_byteStream.write<DeferredCommandFunction>(handler);

    // 3. Write Entity ID.
    this->m_byteStream.write<sge::Entity>(e);

    // 4. Write component payload size.
    uint32_t payloadSize = static_cast<uint32_t>(sizeof(CleanType));
    this->m_byteStream.write<uint32_t>(payloadSize);

    // 5. Write component payload data.
    this->m_byteStream.writeRawBytes(std::addressof(component), payloadSize);

    return *this;
}

template<typename T>
inline sge::CommandBuffer& sge::CommandBuffer::removeComponentDeferred(sge::Entity e)
{
    using CleanType = std::decay_t<T>;

    // 1. Write command type.
    this->m_byteStream.write<CommandType>(CommandType::RemoveComponentDeferred);

    // 2. Write handler.
    auto handler = &sge::CommandBuffer::componentRemovalDispatcher<CleanType>;
    this->m_byteStream.write<DeferredCommandFunction>(handler);

    // 3. Write Entity ID.
    this->m_byteStream.write<sge::Entity>(e);

    return *this;
}

template <typename TCommand>
inline sge::CommandBuffer& sge::CommandBuffer::pushCustomCommand(const TCommand &command)
{
    using CleanType = std::remove_cvref_t<TCommand>;

    static_assert(IsCustomCommand<CleanType>,
    "[CommandBuffer]::pushCustomCommand: Error. Struct must implement 'void execute(sge::CommandContext &context) const'.");

    static_assert(std::is_trivially_copyable_v<CleanType>,
    "[CommandBuffer]::pushCustomCommand: Error. Struct must be trivially copyable.");
    
    // 1. Write command type
    this->m_byteStream.write<CommandType>(CommandType::CustomCommandDeferred);
    
    // 2. Write handler
    CustomCommandFunction handler = [](sge::Registry &registry, const std::vector<sge::Entity>& translationTable, const void *payloadData) {
        const CleanType* userCommand = reinterpret_cast<const CleanType*>(payloadData);
        CommandContext context(registry, translationTable);

        userCommand->execute(context);
    };
    this->m_byteStream.write<CustomCommandFunction>(handler);

    // 3. Write payload size
    uint32_t payloadSize = static_cast<uint32_t>(sizeof(CleanType));
    this->m_byteStream.write<uint32_t>(payloadSize);

    // 4. Write component payload data.
    this->m_byteStream.writeRawBytes(std::addressof(command), payloadSize);
        
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
    
    // 2. Process commands in byte stream.
    this->flushByteStream(registry);
    
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