#pragma once

#include "Player.h"
#include "../misc/InputData.h"
#include "../gui/DebugUI.h"

class PlayerControlScheme {
public:
    int forward, backward, strafeLeft, strafeRight;
    int lookLeft, lookRight, lookUp, lookDown;
    int placeBlock, breakBlock;
    int jump;
    int switchBlockLeft;
    int switchBlockRight;
    int openCmd;
    int crouch;

    PlayerControlScheme();
    PlayerControlScheme(int fwd, int bwd, int sLeft, int sRight,
                        int lLeft, int lRight, int lUp, int lDown,
                        int pBlock, int bBlock, int jmp,
                        int swLeft, int swRight, int cmd, int crch);
};

class PlayerController {
private:

    PlayerControlScheme controlScheme;
    float breakPlaceTimeout;
    bool openedCmd;
    float flyTimer;

public:
    Player* player;
    PlayerController(Player* player);
    void Update(DebugUI* debugUi, InputData input, float dt);

    // Getter/setter if needed
    PlayerControlScheme& GetControlScheme() { return controlScheme; }


    float3 movement;
};