#include <SGE/core/World.h>

sge::World::World(unsigned maxContacts, unsigned iterations) :
m_physicsWorld(maxContacts, iterations)
{
}

void sge::World::setGravity(const sm::Vec3 &gravity)
{
    this->m_physicsWorld.setGravity(gravity);
}

void sge::World::update(sm::real dt)
{
    // Reset force accumulators.
    this->m_physicsWorld.startFrame(this->m_registry);
    
    // Run the physics of the world.
    this->m_physicsWorld.runPhysics(this->m_registry, this->m_cmdBuffer, dt);

    // Flush the command buffer queue.
    // Command buffer is automatically cleared in this function.
    this->m_cmdBuffer.playBack(this->m_registry);
}

sge::CommandBuffer& sge::World::getCommandBuffer()
{
    return this->m_cmdBuffer;
}

sge::Registry& sge::World::getRegistry()
{
    return this->m_registry;
}