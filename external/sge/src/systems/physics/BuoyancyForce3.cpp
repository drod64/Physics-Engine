#include <SGE/systems/physics/BuoyancyForce3.h>

sge::BuoyancyForce3::BuoyancyForce3(sm::real maxDepth, sm::real volume, sm::real waterHeight, sm::real liquidDensity)
{
    this->m_maxDepth = maxDepth;
    this->m_volume = volume;
    this->m_waterHeight = waterHeight;
    this->m_liquidDensity = liquidDensity;
}

void sge::BuoyancyForce3::updateForce(sge::Entity *e, sm::real dt)
{
    // Get necessary component from entity.
    auto &r3 = e->getComponent<sge::CRigidBody3>();

    // Get depth of entity
    sm::real depth = e->getComponent<CTransform3>().position.y;

    // Get top and bottom bounds of entity.
    sm::real top = depth + this->m_maxDepth;
    sm::real bottom = depth - this->m_maxDepth;

    // (Early exit) Check if it's completely above water.
    if (bottom >= this->m_waterHeight) return;

    sm::Vec3 force = {0, 0, 0};
    // Check if entity is completely submerged.
    if (top <= this->m_waterHeight)
    {
        force.y = this->m_liquidDensity * this->m_volume;
    }
    // Otherwise it's partially submerged
    else
    {
        // Get submersion ratio (0 - 1).
        sm::real submersionRatio = (this->m_waterHeight - bottom) / (2 * this->m_maxDepth);

        force.y = this->m_liquidDensity * this->m_volume * submersionRatio;
    }

    r3.addForce(force);
}