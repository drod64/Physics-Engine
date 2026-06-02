#ifndef SGE_FORCE_GENERATOR3_H
#define SGE_FORCE_GENERATOR3_H
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>

namespace sge {
class ForceGenerator3 {
public:

    virtual void updateForce(Entity *e, sm::real dt) = 0;
};
}

#endif // SGE_FORCE_GENERATOR_H