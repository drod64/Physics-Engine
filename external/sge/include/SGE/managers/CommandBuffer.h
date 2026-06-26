#ifndef SGE_COMMAND_BUFFER_H
#define SGE_COMMAND_BUFFER_H
#include <algorithm>
#include <vector>
#include <memory>
#include <SGE/core/ecs/Entity.h>
#include <SGE/managers/Registry.h>
#include <iostream>

namespace sge {
// Forward declaration of Registry class. 
class Registry;

class CommandBuffer {
private:
    struct DeferredComponentPool {
        void *poolData = nullptr;
        void (*playbackFn)(void *, Registry &, const sge::Entity *) = nullptr;
        void (*clearFn)(void*) = nullptr;
        void (*destructFn)(void*) = nullptr;
    };

    // Fake ID counter.
    uint32_t m_placeHolderIndex;

    // Flat vector of pools.
    std::vector<DeferredComponentPool> m_pools; // no reset to save on allocation calls

    // Pending commands.
    std::vector<Entity> m_pendingEntityDestructions;

    //Fake ID ---> Real ID
    std::vector<Entity> m_entityTranslationTable; // no reset to save on allocation calls

public:
    CommandBuffer();
    ~CommandBuffer();

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

    void abort();
};
} // namespace sge

// Implementation

inline sge::CommandBuffer::CommandBuffer()
{
    this->m_placeHolderIndex = 0;

    // Reserve 64 Component pools on startup.
    this->m_pools.reserve(64);
    this->m_entityTranslationTable.reserve(64);
    this->m_pendingEntityDestructions.reserve(64);
}

inline sge::CommandBuffer::~CommandBuffer()
{
    this->abort();
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
inline sge::CommandBuffer& sge::CommandBuffer::addComponentDeferred(sge::Entity e, T&& component)
{
    using ComponentType = std::decay_t<T>;
    const sge::ComponentID ID = sge::ComponentIDCounter::get<ComponentType>();

     // Type erased struct for pool storage.
    struct ConcretePool {
        std::vector<sge::Entity> entities;
        std::vector<ComponentType> components;
    };

    // Resize pools vector.
    if (ID >= this->m_pools.size())
    {
        this->m_pools.resize(ID + 1);
    }

    // Initialize pool.
    if (!this->m_pools[ID].poolData)
    {
        this->m_pools[ID].poolData = new ConcretePool();

        // Playback function.
        this->m_pools[ID].playbackFn = [](void *poolPtr, sge::Registry &registry, const sge::Entity *translationTable) {
            ConcretePool *pool = static_cast<ConcretePool*>(poolPtr);

            // Map fake IDs to real IDs
            const size_t SIZE = pool->entities.size();
            for (size_t i = 0; i < SIZE; ++i)
            {
                sge::Entity e = pool->entities[i];
                
                if (sge::IsFakeEntity(e))
                {
                    e = translationTable[static_cast<uint32_t>(e) & sge::REAL_ENTITY_MASK];
                }

                // Ensure entity was not deleted and still exists in registry.
                if (e != sge::Entity::INVALID && registry.isAlive(e))
                {
                    // Add component to live registry.
                    registry.addComponent<ComponentType>(e, std::move(pool->components[i]));
                }
            }
        };

        // Clear function.
        this->m_pools[ID].clearFn = [](void *poolPtr) {
            ConcretePool* pool = static_cast<ConcretePool*>(poolPtr);
            
            pool->entities.clear();
            pool->components.clear();
        };

        // Destructor function.
        this->m_pools[ID].destructFn = [](void *poolPtr) {
            delete static_cast<ConcretePool*>(poolPtr);
        };
    }
    
    // Push component into deferred pool vector.
    ConcretePool *pool = static_cast<ConcretePool*>(this->m_pools[ID].poolData);
    pool->entities.push_back(e);
    pool->components.push_back(std::forward<T>(component));
    
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
    this->m_placeHolderIndex = 0;
    
    // 2. Create new entities (adding/removing components).
    for (const auto &pool : this->m_pools)
    {
        if (pool.poolData && pool.playbackFn)
        {
            pool.playbackFn(pool.poolData, registry, this->m_entityTranslationTable.data());
        }
    }

    // 3. Clear vector of pending entities and components.
    for (const auto &pool : this->m_pools)
    {
        if (pool.poolData && pool.clearFn)
        {
            pool.clearFn(pool.poolData);
        }
    }
    
    // 4. Process destructions.
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
    this->m_pendingEntityDestructions.clear();
}

inline void sge::CommandBuffer::clear()
{
    this->m_pendingEntityDestructions.clear();
    this->m_placeHolderIndex = 0;
}

inline void sge::CommandBuffer::abort()
{
    for (const auto &pool : this->m_pools)
    {
        if (pool.poolData && pool.destructFn)
        {
            pool.destructFn(pool.poolData);
        }
    }
}

#endif // SGE_COMMAND_BUFFER_H