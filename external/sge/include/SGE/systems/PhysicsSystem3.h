#ifndef SGE_PHYSICS_SYSTEM_H
#define SGE_PHYSICS_SYSTEM_H
#include <SM/Vec3.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>

// Necessary physics components
#include <SGE/components/CAnchorBungee3.h>
#include <SGE/components/CAnchorSpring3.h>
#include <SGE/components/CBungee3.h>
#include <SGE/components/CBuoyancy3.h>
#include <SGE/components/CDrag3.h>
#include <SGE/components/CFakeStiffSpring3.h>
#include <SGE/components/CGravity3.h>
#include <sge/components/CRigidBody3.h>
#include <sge/components/CSpring3.h>
#include <sge/components/CTransform3.h>

namespace sge {
class PhysicsSystem3 {
public:
    void operator () (Registry &registry, CommandBuffer &, sm::real dt);
    
private:
    /**
     * Resets force accumulators of all Entity objects. 
     */
    void startFrame(Registry &registry);
    
    /**
     * Integrates all the entities in this world by the given delta time.
     * @param dt the delta time between frames
     */
    void integrate(Registry &registry, sm::real dt);
};
}

#endif // SGE_PHYSICS_SYSTEM_H
