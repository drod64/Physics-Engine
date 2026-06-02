#include <SGE/systems/physics/ForceRegistry3.h>

sge::ForceRegistry3::ForceRegistry3() :
m_registrations()
{}

void sge::ForceRegistry3::add(sge::Entity *e, sge::ForceGenerator3 *fg)
{
    this->m_registrations.push_back({e, fg});
}

void sge::ForceRegistry3::remove(sge::Entity *e, sge::ForceGenerator3 *fg)
{
    int index = 0;
    
    while (index < this->m_registrations.size())
    {
        // Check if current registration matches criteria
        if ((this->m_registrations.at(index).e == e) && (this->m_registrations.at(index).fg == fg))
        {
            // Swap targeted registration with last registration
            this->m_registrations.at(index) = this->m_registrations.back();
            
            // Delete target registration
            this->m_registrations.pop_back();
        }
        else
        {
            ++index;
        }
    }
}

void sge::ForceRegistry3::removeEntity(sge::Entity *e)
{
    // Locate ALL registrations that contain Entity to be removed
    auto newEnd = std::remove_if(this->m_registrations.begin(), this->m_registrations.end(), [e] (const ForceRegistration3 &fr) -> bool {
        return fr.e == e;
    });

    // Erase Entity/Entities
    this->m_registrations.erase(newEnd, this->m_registrations.end());
}

void sge::ForceRegistry3::clear()
{
    this->m_registrations.clear();
}

void sge::ForceRegistry3::updateForces(sm::real dt)
{
    Registry3::iterator i = this->m_registrations.begin();

    for (i; i != this->m_registrations.end(); ++i)
    {
        i->fg->updateForce(i->e, dt);
    }
}