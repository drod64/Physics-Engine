#ifndef SGE_WORLD3_H
#define SGE_WORLD3_H
#include <tuple>
#include <vector>
#include <type_traits>
#include <raylib.h>
#include <SM/Vec3.h>
#include <SM/Precision.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>

namespace sge {

class World {
protected:
    Registry                        m_registry;
    CommandBuffer                   m_cmdBuffer;
    Camera3D                        m_camera;

public:
    /**
     * Parameterized Constructor.
     * @param maxContacts the max amount of contacts to be handled per frame
     * @param iterations the number of iterations allowed for contact-resolution. If not explicitly stated, iterations = maxContacts * 2
     */
    World(unsigned maxContacts, unsigned iterations = 0);

    void setGravity(const sm::Vec3 &gravity);

    /**
     * Updates the World.
     */
    void update(sm::real dt);

    CommandBuffer& getCommandBuffer();

    Registry& getRegistry();

    Camera3D& getCamera();

};
}

#endif // SGE_WORLD_H