#pragma once

#include "Player.h"
#include "../misc/InputData.h"
#include "../gui/DebugUI.h"
//mclib
#include <mclib/common/Vector.h>

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
    float m_breakPlaceTimeout = 0.f;
    bool m_openedCmd = false;
    float m_flyTimer = -1.f;

public:
    Player* player;
    PlayerController(Player* player);
    void Update(DebugUI* debugUi, InputData input, float dt);

    // Getter/setter if needed
    PlayerControlScheme& GetControlScheme() { return m_controlScheme; }


    mc::Vector3d movement;
};