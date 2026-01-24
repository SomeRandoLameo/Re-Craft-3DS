#pragma once

#include "input/PadAngles.hpp"

#define PAD_MAX 156
#define PAD_MIN 30 // move to settings
#define PAD_MAXF (float)PAD_MAX

class PlayerInput {
public:
    struct Data {
        PadAngles move;
        PadAngles look;
        PadAngles movePrev;
        PadAngles lookPrev;
    } data;
    bool isMoved;
    bool isLooked;

    void update();

    PlayerInput();
};
