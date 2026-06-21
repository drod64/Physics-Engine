#ifndef SGE_CJOINT_H
#define SGE_CJOINT_H
#include <exception>
#include <cstring>
#include <array>
#include <SGE/entities/Entity.h>
#include <SGE/components/physics/JointTypeID.h>

namespace sge {
    /**
     * Struct for jointed/linked entities.
     */
    struct CJoint {
        JointTypeID typeID;
        Entity targetEntity;
        alignas(8) std::array<uint8_t, 24> payload;

        template <typename T>
        void setParams(const T &params)
        {
            if (sizeof(T) > 24)
            {
                throw std::runtime_error("Joint parameters exceed payload size! [" + payload.size() + "]");
            }

            std::memcpy(payload.data(), &params, sizeof(T));
        }

        template <typename T>
        const T& getParams() const
        {
            return *reinterpret_cast<const T*>(payload.data());
        }

        template <typename T>
        static CJoint create(JointTypeID type, Entity targetEntity, const T &params)
        {
            CJoint joint;
            joint.typeID = type;
            joint.targetEntity = targetEntity;
            joint.setParams(params);

            return joint;
        }
    };
}

#endif // SGE_CJOINT_H