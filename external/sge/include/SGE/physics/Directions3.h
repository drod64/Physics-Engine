#ifndef SGE_DIRECTIONS3_H
#define SGE_DIRECTIONS3_H
#include <SM/Vec3.h>

namespace sge {
    namespace Directions3 {
        inline constexpr sm::Vec3 GLOBAL_RIGHT      {static_cast<sm::real>(1), static_cast<sm::real>(0), static_cast<sm::real>(0)};
        inline constexpr sm::Vec3 GLOBAL_UP         {static_cast<sm::real>(0), static_cast<sm::real>(1), static_cast<sm::real>(0)};
        inline constexpr sm::Vec3 GLOBAL_FORWARD    {static_cast<sm::real>(0), static_cast<sm::real>(0), static_cast<sm::real>(-1)};
        
        inline constexpr sm::Vec3 GLOBAL_LEFT       {static_cast<sm::real>(-1), static_cast<sm::real>(0), static_cast<sm::real>(0)};
        inline constexpr sm::Vec3 GLOBAL_DOWN       {static_cast<sm::real>(0), static_cast<sm::real>(-1), static_cast<sm::real>(0)};
        inline constexpr sm::Vec3 GLOBAL_BACKWARDS  {static_cast<sm::real>(0), static_cast<sm::real>(0), static_cast<sm::real>(1)};

        inline constexpr sm::real getRightSign()
        {
            return GLOBAL_RIGHT.x;
        }
        
        inline constexpr sm::real getUpSign()
        {
            return GLOBAL_UP.y;
        }
        
        inline constexpr sm::real getForwardSign()
        {
            return GLOBAL_FORWARD.z;
        }

        inline constexpr sm::real getLeftSign()
        {
            return GLOBAL_LEFT.x;
        }
        
        inline constexpr sm::real getDownSign()
        {
            return GLOBAL_DOWN.y;
        }
        
        inline constexpr sm::real getBackwardSign()
        {
            return GLOBAL_BACKWARDS.z;
        }
    }
}

#endif // SGE_DIRECTIONS3_H