#ifndef SGE_FORCE_REGISTRY3_H
#define SGE_FORCE_REGISTRY3_H
#include <algorithm>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class ForceRegistry3 {
protected:
    struct ForceRegistration3 {
        Entity *e;
        ForceGenerator3 *fg;
    };

    typedef std::vector<ForceRegistration3> Registry3;
    Registry3 m_registrations;

public:
    ForceRegistry3();

    void add(Entity *e, ForceGenerator3 *fg);
    void remove(Entity *e, ForceGenerator3 *fg);
    void removeEntity(Entity *e);

    void clear();

    void updateForces(sm::real dt);
};
}

#endif // SGE_FORCE_REGISTRY3_H