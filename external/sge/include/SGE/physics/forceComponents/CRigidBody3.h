#ifndef SGE_CRIGID_BODY3_H
#define SGE_CRIGID_BODY3_H
#include <SM/Vec3.h>

namespace sge {
/**
 * Component that stores rigid body data for a 3D environment.
 */
class CRigidBody3 {
public:
    bool is_static = false;
    sm::real inverseMass = 0;;
    sm::Vec3 velocity = {0, 0, 0};
    sm::Vec3 accumulatedForce = {0, 0, 0};

    /**
     * Parameterized Constructor.
     * @param vel the velocity of the rigid body
     * @param accumulatedForce the accumulated force of the rigid body
     * @param mass the mass of the rigid body
     */
    CRigidBody3(sm::real mass, bool isStatic = false);

    /**
     * Sets the mass of the rigid body.
     * @param mass the new mass of the rigid body
     */
    void setMass(sm::real mass);

    /**
     * Retrieves the mass of the rigid body.
     * @return the mass of the rigid body
     */
    sm::real getMass() const;

    /**
     * Adds a force to the rigid body.
     * @param force the force to add to the rigid body
     */
    void addForce(const sm::Vec3 &force);

    /**
     * Clears all forces taking effect on the rigid body.
     */
    void clearAccumulator();
}; // class CRigidBody3
} // namespace sge

#endif // SGE_CRIGID_BODY3_H