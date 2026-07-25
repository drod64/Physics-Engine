#ifndef TESTING_ENGINE_CAMERA_FOLLOW_SYSTEM_H
#define TESTING_ENGINE_CAMERA_FOLLOW_SYSTEM_H
#include <SM/MathUtil.h>
#include <SM/Orientations.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Reads
#include <TestingEngine/components/CCameraFollow3.h>
#include <TestingEngine/components/CCameraControl3.h>

// Writes
#include <SGE/core/ecs/components/CTransform3.h>

// Tags (no action necessary)
#include <TestingEngine/components/TagFPSCam.h>
#include <TestingEngine/components/TagThirdPersonCam.h>


namespace CameraFollowSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt);

    sge::SystemDescriptor getSystemDescription();
} // namespace CameraFollowSystem

#endif // TESTING_ENGINE_CAMERA_FOLLOW_SYSTEM_H