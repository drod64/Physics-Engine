#ifndef ACTION_SCENE_PLAY
#define ACTION_SCENE_PLAY
#include <SGE/util/Action.h>

enum GameplayAction : sge::ActionID {
    PAUSE,
    QUIT,
    TOGGLE_GRID,
    LEFT,
    JUMP,
    RIGHT,
    DOWN,
    SHOOT_PISTOL,
    SHOOT_ARTILLERY,
    SHOOT_FIREBALL,
    SHOOT_LASER,
};

#endif // ACTION_SCENE_PLAY