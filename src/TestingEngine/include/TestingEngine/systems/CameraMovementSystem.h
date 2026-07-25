#ifndef TESTING_ENGINE_FPS_CAMERA_SYSTEM_H
#define TESTING_ENGINE_FPS_CAMERA_SYSTEM_H
#include <SM/Precision.h>
#include <SM/Quaternion.h>
#include <SM/Orientations.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Reads.
#include <SGE/core/globalContext/contexts/PlayerInputContext.h>

// Writes
#include <TestingEngine/components/CCameraControl3.h>

namespace CameraMovementSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt);

    sge::SystemDescriptor getSystemDescription();
};

#endif // TESTING_ENGINE_FPS_CAMERA_SYSTEM_H