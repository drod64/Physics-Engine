#ifndef SCENE_PLAY_ACTIONS_H
#define SCENE_PLAY_ACTIONS_H
#include <cstdint>

enum class ScenePlayAction : uint32_t {
    MoveForward,
    MoveLeft,
    MoveBackward,
    MoveRight,
    MoveUp,
    MoveDown,

    SpawnAnchorBungee,
    SpawnAnchorSpring,
    SpawnBungeeSpring,
    SpawnBuoyant,
    SpawnFakeStiffSpring,
    SpawnSpring,

    DeleteEntities,
    RemoveGravity,

    Pause,
};

#endif // SCENE_PLAY_ACTIONS_H