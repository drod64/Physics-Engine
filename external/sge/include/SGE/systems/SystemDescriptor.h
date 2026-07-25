#ifndef SGE_SYSTEM_DESCRIPTOR_H
#define SGE_SYSTEM_DESCRIPTOR_H
#include <cstdint>
#include <bitset>
#include <string>
#include <SM/Precision.h>

namespace sge {
    // Forwarded classes.
    class Registry;
    class CommandBuffer;

    constexpr size_t MAX_MASK_SIZE = 100;
    using Mask = std::bitset<MAX_MASK_SIZE>;
    using SystemFn = void(*)(Registry &, CommandBuffer &, sm::real);

    // 2. Execution phase of systems. Will be used to sort them.
    enum class ExecutionPhase : uint32_t {
        PreUpdate = 0,      
        Gameplay,           // Gameplay systems
        PhysicsClear,       // Clear force accumulation
        PhysicsForceGen,    // Physics force generation
        PhysicsIntegrate,   // Physics integration 
        PostGameplay,
        PostUpdate          // Rendering.
    };

    struct AccessMasks {
        Mask reads;
        Mask writes;
        Mask accumulates;
        Mask requirez;
        Mask withouts;
    };
    
    // 3. Struct that holds the description of a System.
    struct SystemDescriptor {
        ExecutionPhase phase;
        SystemFn functionPtr;
        AccessMasks components;
        AccessMasks contexts;
        
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