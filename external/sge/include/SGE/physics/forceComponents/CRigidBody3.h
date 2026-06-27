#ifndef SGE_CRIGID_BODY3_H
#define SGE_CRIGID_BODY3_H
#include <SM/Vec3.h>

namespace sge {
/**
 * Component that stores rigid body data for a 3D environment.
 */
class CRigidBody3 {
public:
    sm::Vec3 velocity = {0, 0, 0};
    sm::Vec3 accumulatedForce = {0, 0, 0};
    sm::real damping = 0;

    CRigidBody3() = default;

    /**
     * Parameterized Constructor.
     * @param vel the velocity of the rigid body
     * @param accumulatedForce the accumulated force of the rigid body
     * @param mass the mass of the rigid body
     * @param damping the damping scalar of the rigid body
     */
    CRigidBody3(const sm::Vec3 &vel, const sm::Vec3 &accumulatedForce, sm::real mass, sm::real damping);

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
     * Retrieves the inverse mass of the rigid body.
     * @return the inverse mass of the rigid body
     */
    sm::real getInverseMass() const;

    /**
     * Sets the rigid body as static depending on value.
     * @param value true or false
     */
    void setStatic(bool value);

    /**
     * Checks if the rigid body is static.
     * @return true if the body is static, false otherwise
     */
    bool isStatic() const;

    /**
     * Adds a force to the rigid body.
     * @param force the force to add to the rigid body
     */
    void addForce(const sm::Vec3 &force);

    /**
     * Clears all forces taking effect on the rigid body.
     */
    void clearAccumulator();


private:
    sm::real m_inverseMass;
    bool m_static = false;

};
} // namespace sge


#endif // SGE_CRIGID_BODY3_H