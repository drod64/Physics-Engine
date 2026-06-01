#ifndef SGE_CIGID_BODY3_H
#define SGE_CRIGID_BODY3_H
#include <SM/Vec3.h>
#include <SGE/components/Component.h>

namespace sge {
class CRigidBody3 : public Component {
public:
    sm::Vec3 velocity;
    sm::Vec3 accumulatedForce;
    sm::real damping;

    CRigidBody3();
    CRigidBody3(const sm::Vec3 &vel, const sm::Vec3 &accumulatedForce, sm::real mass, sm::real damping);

    void setMass(sm::real mass);
    sm::real getMass() const;

    void addForce(const sm::Vec3 &force);
    void clearAccumulator();

private:
    sm::real m_inverseMass;

};
} // namespace sge


#endif // SGE_CRIGID_BODY3_H