#ifndef SGE_SYSTEM_DESCRIPTOR_H
#define SGE_SYSTEM_DESCRIPTOR_H
#include <cstdint>
#include <bitset>
#include <string>
#include <SM/Precision.h>
#include <SGE/core/ecs/components/ComponentIDCounter.h>
#include <SGE/core/globalContext/GlobalContextIDCounter.h>

namespace sge {
    // Forwarded classes.
    class Registry;
    class CommandBuffer;

    // 2. Execution phase of systems. Will be used to sort them.
    enum class ExecutionPhase : uint32_t {
        PreUpdate = 0,      
        Gameplay,           // Gameplay systems
        PostGameplay,
        PhysicsClear,       // Clear force accumulation
        PhysicsForceGen,    // Physics force generation
        PhysicsIntegrate,   // Physics integration 
        PostUpdate          // Rendering.
    };
    
    using SystemFn = void(*)(Registry &, CommandBuffer &, sm::real);

    // 3. Struct that holds the description of a System.
    struct SystemDescriptor {
        ExecutionPhase phase;
        SystemFn functionPtr;
        ComponentMask componentReads;
        ComponentMask componentWrites;
        ComponentMask componentAccumulates;
        GlobalContextMask contextReads;
        GlobalContextMask contextWrites;
        GlobalContextMask contextAccumulates;
        std::string name;

        SystemDescriptor()
        {
            // Fallback phase.
            this->phase = ExecutionPhase::Gameplay;
            this->functionPtr = nullptr;
        }
    }; // struct SystemDescriptor
}

#endif // SGE_SYSTEM_DESCRIPTOR_H