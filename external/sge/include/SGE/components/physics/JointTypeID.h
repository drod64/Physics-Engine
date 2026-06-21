#ifndef SGE_JOINT_TYPE_ID_H
#define SGE_JOINT_TYPE_ID_H
#include <cstdint>

namespace sge {
    /**
     * Unique IDs for joints/links. 
     */
    enum class JointTypeID : uint16_t {
        Bungee,
        Cable,
        Rod,
        Spring
    };
}

#endif // SGE_JOINT_TYPE_ID_H