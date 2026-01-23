#pragma once

class Player;

#include "../gui/DebugUI.hpp"
#include "../misc/InputData.hpp"
#include "Player.hpp"
//mclib
#include <mclib/common/Vector.h>

class Input {
public:
    float keys[23];
    bool keysup[23];
    bool keysdown[23];

    float IsKeyDown(int combo, Input *input) {
        return input->keys[combo];
    }

    bool WasKeyReleased(int combo, Input *input) {
        return input->keysup[combo];
    }

    bool WasKeyPressed(int combo, Input *input) {
        return input->keysdown[combo];
    }
};

void convertPlatformInput(InputData *input, float ctrls[], bool keysdown[], bool keysup[]);

class PlayerControlScheme {
public:
    int forward = 0;
    int backward = 0;
    int strafeLeft = 0;
    int strafeRight = 0;
    int lookLeft = 0;
    int lookRight = 0;
    int lookUp = 0;
    int lookDown = 0;
    int placeBlock = 0;
    int breakBlock = 0;
    int jump = 0;
    int switchBlockLeft = 0;
    int switchBlockRight = 0;
    int openCmd = 0;
    int crouch = 0;

    PlayerControlScheme() = default;
    PlayerControlScheme(int fwd, int bwd, int sLeft, int sRight,
                        int lLeft, int lRight, int lUp, int lDown,
                        int pBlock, int bBlock, int jmp,
                        int swLeft, int swRight, int cmd, int crch);
};



class PlayerController {
private:
    PlayerControlScheme m_controlScheme;
public:
    Player* player;
    PlayerController(Player* player);

    // Getter/setter if needed
    PlayerControlScheme& GetControlScheme() { return m_controlScheme; }

    mc::Vector3d movement;
};