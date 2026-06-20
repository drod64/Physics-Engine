#ifndef SGE_SYSTEM_DESCRIPTOR_H
#define SGE_SYSTEM_DESCRIPTOR_H
#include <cstdint>
#include <bitset>
#include <string>
#include <SM/Precision.h>
#include <SGE/core/registryResources/ResourceRegistry.h>

namespace sge {
    // Forwarded classes.
    class Registry;
    class CommandBuffer;

    // 1. Unique IDs for every Component type.
    using ComponentID = uint32_t;
    struct ComponentIDCounter {
    private:
        /**
         * Helper function to keep IDs unique.
         * For every different Component type, this function will be called.
         */
        static inline ComponentID nextID()
        {
            static ComponentID counter = 0;
            return counter++;
        }

    public:
        /**
         * Fetches a unique ID based on T.
         * @tparam the Component data type
         * @return a unique ID based on the Component data type. Will return the same unique ID for Components of the same type
         */
        template <typename T>
        static ComponentID get()
        {
            static const ComponentID ID = nextID();
            return ID;
        }
    }; // class ComponentIDCounter

    using SystemFn = void(*)(Registry &, CommandBuffer &, sm::real);
    constexpr size_t MAX_COMPONENTS = 100;
    using ComponentMask = std::bitset<MAX_COMPONENTS>;

    // 2. Execution phase of systems. Will be used to sort them.
    enum class ExecutionPhase : uint32_t {
        PreUpdate = 0,      
        Gameplay,           // Gameplay systems
        PhysicsClear,       // Clear force accumulation
        PhysicsForceGen,    // Physics force generation
        PhysicsIntegrate,   // Physics integration 
        PostUpdate          // Rendering.
    };

    // 3. Struct that holds the description of a System.
    struct SystemDescriptor {
        ExecutionPhase phase;
        SystemFn functionPtr;
        ComponentMask componentReads;
        ComponentMask componentWrites;
        ResourceMask resourceReads;
        ResourceMask resourceWrites;
        std::string name;

        SystemDescriptor()
        {
            // Fallback phase.
            this->phase = ExecutionPhase::Gameplay;
            this->functionPtr = nullptr;

            // No initialization needed for these.
            // ComponentMask componentReads;
            // ComponentMask componentWrites;
            // ResourceMask resourceReads;
            // ResourceMask resourceWrites;
            // std::string name;
        }

        SystemDescriptor(ExecutionPhase phase, SystemFn functionPtr, ComponentMask componentReads, ComponentMask componentWrites,
                        ResourceMask resourceReads, ResourceMask resourceWrites, const std::string &name)
        {
            this->phase = phase;
            this->functionPtr = functionPtr;
            this->componentReads = componentReads;
            this->componentWrites = componentWrites;
            this->resourceReads = resourceReads;
            this->resourceWrites = resourceWrites;
            this->name = std::move(name);
        }
    }; // struct SystemDescriptor
}

#endif // SGE_SYSTEM_DESCRIPTOR_H