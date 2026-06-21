#ifndef SGE_CFORCE_H
#define SGE_CFORCE_H
#include <array>
#include <cstring>
#include <exception>
#include <SGE/components/physics/ForceTypeID.h>

// ECS Accumulator-Constraint Pipeline.
namespace sge {
    /**
     * Struct for non-jointed force generation.
     * (Ex. Gravity, Drag, Anchored spring/bungee).
     */
    struct CForce {
        ForceTypeID typeID;
        alignas(16) std::array<uint8_t, 32> payload;

        template <typename T>
        void setParams(const T &params)
        {
            if (sizeof(T) > 32)
            {
                throw std::runtime_error("Parameters exceed maximum payload size! [" + payload.size() + "]");
            }
            
            std::memcpy(payload.data(), &params, sizeof(T));
        }

        template <typename T>
        const T& getParams() const
        {
            return *reinterpret_cast<const T*>(payload.data());
        }

        template <typename T>
        static CForce create(ForceTypeID type, const T &params)
        {
            CForce force;
            force.typeID = type;
            force.setParams(params);

            return force;
        }
    };
}

#endif // SGE_CPHYSICAL_FORCE_H