#include <SGE/systems/physics/BuoyancyForce3.h>
#include <iostream>

sge::BuoyancyForce3::BuoyancyForce3(sm::real maxDepth, sm::real volume, sm::real waterHeight, sm::real liquidDensity)
{
    this->m_maxDepth = maxDepth;
    this->m_volume = volume;
    this->m_waterHeight = waterHeight;
    this->m_liquidDensity = liquidDensity;
}

void sge::BuoyancyForce3::updateForce(sge::Entity *e, sm::real dt)
{
    auto &r3 = e->getComponent<sge::CRigidBody3>();

    // Get depth of entity
    sm::real depth = e->getComponent<CTransform3>().position.y;
    // Check if it's above water
    if (depth >= this->m_waterHeight + this->m_maxDepth) return;

    sm::Vec3 force = {0, 0, 0};
    if (depth <= this->m_waterHeight - this->m_maxDepth)
    {
        force.y = this->m_liquidDensity * this->m_volume;
        r3.addForce(force);
        return;
    }

    force.y = this->m_liquidDensity * this->m_volume *
                (depth - this->m_maxDepth - this->m_waterHeight) / 2 * this->m_maxDepth;

    r3.addForce(force);

    std::cout << "Force: {" << force.x << ", " << force.y << ", " << force.z << "}\n";
}