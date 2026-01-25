#include "input/PlayerInput.hpp"

#include "input/InputManager.hpp"

PlayerInput::PlayerInput() {
}

static inline u8 padToMatrix(InputAction act, bool isPlus) {
    s16 axis;

    // get numbers
    switch (Input::getKey(act)) {
    case KEY_CPAD_UP:
    case KEY_CPAD_DOWN:
        axis = Input::getPad().dy;
        break;
    case KEY_CPAD_LEFT:
    case KEY_CPAD_RIGHT:
        axis = Input::getPad().dx;
        break;
    case KEY_CSTICK_UP:
    case KEY_CSTICK_DOWN:
        axis = Input::getStick().dy;
        break;
    case KEY_CSTICK_LEFT:
    case KEY_CSTICK_RIGHT:
        axis = Input::getStick().dx;
        break;
    default:
        return Input::isPressed(act) ? 180 : 0;
    }

    // convert numbers
    if (isPlus) {
        return axis > PAD_MIN ? axis : 0;
    } else {
        return axis < -PAD_MIN ? -axis : 0;
    }
}

void PlayerInput::update() {
    data.movePrev   = data.move;
    data.move.up    = padToMatrix(INP_MOVE_FORWARD, true);
    data.move.down  = padToMatrix(INP_MOVE_BACK, false);
    data.move.left  = padToMatrix(INP_MOVE_LEFT, false);
    data.move.right = padToMatrix(INP_MOVE_RIGHT, true);
    isMoved         = data.movePrev != data.move;

    data.lookPrev   = data.look;
    data.look.up    = padToMatrix(INP_LOOK_UP, true);
    data.look.down  = padToMatrix(INP_LOOK_DOWN, false);
    data.look.left  = padToMatrix(INP_LOOK_LEFT, false);
    data.look.right = padToMatrix(INP_LOOK_RIGHT, true);
    isLooked        = data.look != 0;
}
