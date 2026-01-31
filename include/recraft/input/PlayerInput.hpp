#pragma once

#include "input/PadAngles.hpp"

#define PAD_MAX 156
#define PAD_MIN 30 // move to settings
#define PAD_MAXF (float)PAD_MAX

class PlayerInput {
public:
    struct Data {
        PadAngles movePrev;
        PadAngles move;
        PadAngles look;
    };

    Data data;

    bool isMoving   = true;
    bool isMoveDiff = true;
    bool isLooking  = true;

    void update();

    PlayerInput();
};
