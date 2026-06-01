#include <SPE/Particle3ForceRegistry.h>

spe::Particle3ForceRegistry::Particle3ForceRegistry()
:
m_registrations()
{}

void spe::Particle3ForceRegistry::add(spe::Particle3 *particle, spe::Particle3ForceGenerator *fg)
{
    this->m_registrations.push_back({particle, fg});
}

void spe::Particle3ForceRegistry::remove(spe::Particle3 *particle, spe::Particle3ForceGenerator *fg)
{
    std::remove_if(this->m_registrations.begin(), this->m_registrations.end(), [&] (Particle3ForceRegistration r) -> bool {
        r.particle = particle;
        r.fg = fg;
    });
}

void spe::Particle3ForceRegistry::clear()
{
    this->m_registrations.clear();
}

void spe::Particle3ForceRegistry::updateForces(sm::real dt)
{
    Registry::iterator i = this->m_registrations.begin();

    for (i; i != this->m_registrations.end(); ++i)
    {
        i->fg->updateForce(i->particle, dt);
    }
}