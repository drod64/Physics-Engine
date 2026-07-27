#ifndef SGE_CONFIG_H
#define SGE_CONFIG_H
#include <SM/Vec3.h>
#include <SM/Quaternion.h>
#include <SM/Orientations.h>

namespace sge {
    struct Axes {
        constexpr static sm::Vec3 GLOBAL_RIGHT      {static_cast<sm::real>(1),  static_cast<sm::real>(0), static_cast<sm::real>(0)};
        constexpr static sm::Vec3 GLOBAL_UP         {static_cast<sm::real>(0),  static_cast<sm::real>(1), static_cast<sm::real>(0)};
        constexpr static sm::Vec3 GLOBAL_FORWARD    {static_cast<sm::real>(0),  static_cast<sm::real>(0), static_cast<sm::real>(-1)};

        constexpr static sm::real DETERMINANT = 
            (GLOBAL_RIGHT.x * (GLOBAL_UP.y * GLOBAL_FORWARD.z - GLOBAL_UP.z * GLOBAL_FORWARD.y)) -
            (GLOBAL_RIGHT.y * (GLOBAL_UP.x * GLOBAL_FORWARD.z - GLOBAL_UP.z * GLOBAL_FORWARD.x)) +
            (GLOBAL_RIGHT.z * (GLOBAL_UP.x * GLOBAL_FORWARD.y - GLOBAL_UP.y * GLOBAL_FORWARD.x));

        constexpr static sm::real HANDEDNESS_SIGN = (DETERMINANT >= static_cast<sm::real>(0)) ?
                                                                        static_cast<sm::real>(1) : static_cast<sm::real>(-1);

        static_assert(Axes::HANDEDNESS_SIGN == static_cast<sm::real>(-1), 
                "CRITICAL CONFIG ERROR: Engine must be configured as a LEFT-HANDED basis!");

        constexpr static sm::real getRightSign()
        {
            return GLOBAL_RIGHT.x;
        }

        constexpr static sm::real getUpSign()
        {
            return GLOBAL_UP.y;
        }

        constexpr static sm::real getForwardSign()
        {
            return GLOBAL_FORWARD.z;
        }
    }; // struct Axes
} // namespace sge

#endif // SGE_CONFIG_H