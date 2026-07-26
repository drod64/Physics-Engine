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
    }
}

#endif // SGE_DIRECTIONS3_H