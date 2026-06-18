#include <SGE/core/World.h>

sge::World::World(unsigned maxContacts, unsigned iterations)
{
}

void sge::World::setGravity(const sm::Vec3 &gravity)
{
    
}

void sge::World::update(sm::real dt)
{
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

Camera3D& sge::World::getCamera()
{
    return this->m_camera;
}