#include "entity/PlayerController.h"

#include "misc/CommandLine.h"
#include "misc/NumberUtils.h"

#include <unistd.h>

extern "C" {
#include <ini/ini.h>
}

#include <3ds.h>

const char* platform_key_names[23] = {
    "Not Set", "A", "B", "X", "Y", "L", "R", "Start", "Select",
    "DUp", "DDown", "DLeft", "DRight", "CircUp", "CircDown", "CircLeft",
    "CircRight", "CStickUp", "CStickDown", "CStickLeft", "CStickRight", "ZL", "ZR"
};

enum {
    K3DS_Undefined = 0,
    K3DS_A, K3DS_B, K3DS_X, K3DS_Y,
    K3DS_L, K3DS_R, K3DS_START, K3DS_SELECT,
    K3DS_DUP, K3DS_DDOWN, K3DS_DLEFT, K3DS_DRIGHT,
    K3DS_CPAD_UP, K3DS_CPAD_DOWN, K3DS_CPAD_LEFT, K3DS_CPAD_RIGHT,
    K3DS_CSTICK_UP, K3DS_CSTICK_DOWN, K3DS_CSTICK_LEFT, K3DS_CSTICK_RIGHT,
    K3DS_ZL, K3DS_ZR
};

// Default control schemes
static const PlayerControlScheme platform_default_scheme(
        K3DS_CPAD_UP,      // forward
        K3DS_CPAD_DOWN,    // backward
        K3DS_CPAD_LEFT,    // strafeLeft
        K3DS_CPAD_RIGHT,   // strafeRight
        K3DS_Y,            // lookLeft
        K3DS_A,            // lookRight
        K3DS_X,            // lookUp
        K3DS_B,            // lookDown
        K3DS_L,            // placeBlock
        K3DS_R,            // breakBlock
        K3DS_DUP,          // jump
        K3DS_DLEFT,        // switchBlockLeft
        K3DS_DRIGHT,       // switchBlockRight
        K3DS_SELECT,       // openCmd
        K3DS_DDOWN         // crouch
);

static const PlayerControlScheme n3ds_default_scheme(
        K3DS_CPAD_UP,      // forward
        K3DS_CPAD_DOWN,    // backward
        K3DS_CPAD_LEFT,    // strafeLeft
        K3DS_CPAD_RIGHT,   // strafeRight
        K3DS_CSTICK_LEFT,  // lookLeft
        K3DS_CSTICK_RIGHT, // lookRight
        K3DS_CSTICK_UP,    // lookUp
        K3DS_CSTICK_DOWN,  // lookDown
        K3DS_ZL,           // placeBlock
        K3DS_ZR,           // breakBlock
        K3DS_A,            // jump
        K3DS_L,            // switchBlockLeft
        K3DS_R,            // switchBlockRight
        K3DS_SELECT,       // openCmd
        K3DS_Y             // crouch
);

typedef struct {
    float keys[23];
    bool keysup[23];
    bool keysdown[23];
} PlatformAgnosticInput;

static void convertPlatformInput(InputData* input, float ctrls[], bool keysdown[], bool keysup[]) {
    ctrls[0] = 0.f;
    keysdown[0] = 0;
    keysup[0] = 0;

    // Binary keys
    const struct {
        int index;
        u32 key;
    } binaryKeys[] = {
            {K3DS_A, KEY_A},
            {K3DS_B, KEY_B},
            {K3DS_X, KEY_X},
            {K3DS_Y, KEY_Y},
            {K3DS_L, KEY_L},
            {K3DS_R, KEY_R},
            {K3DS_START, KEY_START},
            {K3DS_SELECT, KEY_SELECT},
            {K3DS_DUP, KEY_DUP},
            {K3DS_DDOWN, KEY_DDOWN},
            {K3DS_DLEFT, KEY_DLEFT},
            {K3DS_DRIGHT, KEY_DRIGHT},
            {K3DS_ZL, KEY_ZL},
            {K3DS_ZR, KEY_ZR}
    };

    for (int i = 0; i < 14; i++) {
        int idx = binaryKeys[i].index;
        u32 key = binaryKeys[i].key;
        ctrls[idx] = (float)((input->keysdown & key) || (input->keysheld & key));
        keysdown[idx] = input->keysdown & key;
        keysup[idx] = input->keysup & key;
    }

    // Circle pad
    float circX = (float)input->circlePadX / 0x9c;
    float circY = (float)input->circlePadY / 0x9c;

    const struct {
        int index;
        float value;
        u32 key;
    } circlePadKeys[] = {
            {K3DS_CPAD_UP, circY, KEY_CPAD_UP},
            {K3DS_CPAD_DOWN, circY, KEY_CPAD_DOWN},
            {K3DS_CPAD_LEFT, circX, KEY_CPAD_LEFT},
            {K3DS_CPAD_RIGHT, circX, KEY_CPAD_RIGHT}
    };

    for (int i = 0; i < 4; i++) {
        int idx = circlePadKeys[i].index;
        float val = circlePadKeys[i].value;
        u32 key = circlePadKeys[i].key;
        ctrls[idx] = ABS(val) * (float)((input->keysdown & key) || (input->keysheld & key));
        keysdown[idx] = input->keysdown & key;
        keysup[idx] = input->keysup & key;
    }

    // C-stick
    float cstickX = (float)input->cStickX / 0x9c;
    float cstickY = (float)input->cStickY / 0x9c;

    const struct {
        int index;
        float value;
        u32 key;
    } cstickKeys[] = {
            {K3DS_CSTICK_UP, cstickY, KEY_CSTICK_UP},
            {K3DS_CSTICK_DOWN, cstickY, KEY_CSTICK_DOWN},
            {K3DS_CSTICK_LEFT, cstickX, KEY_CSTICK_LEFT},
            {K3DS_CSTICK_RIGHT, cstickX, KEY_CSTICK_RIGHT}
    };

    for (int i = 0; i < 4; i++) {
        int idx = cstickKeys[i].index;
        float val = cstickKeys[i].value;
        u32 key = cstickKeys[i].key;
        ctrls[idx] = ABS(val) * (float)((input->keysdown & key) || (input->keysheld & key));
        keysdown[idx] = input->keysdown & key;
        keysup[idx] = input->keysup & key;
    }
}

// Utility functions
static inline float IsKeyDown(int combo, PlatformAgnosticInput* input) {
    return input->keys[combo];
}

static inline bool WasKeyReleased(int combo, PlatformAgnosticInput* input) {
    return input->keysup[combo];
}

static inline bool WasKeyPressed(int combo, PlatformAgnosticInput* input) {
    return input->keysdown[combo];
}

// PlayerControlScheme implementation
PlayerControlScheme::PlayerControlScheme()
        : forward(0), backward(0), strafeLeft(0), strafeRight(0),
          lookLeft(0), lookRight(0), lookUp(0), lookDown(0),
          placeBlock(0), breakBlock(0), jump(0),
          switchBlockLeft(0), switchBlockRight(0),
          openCmd(0), crouch(0) {
}

PlayerControlScheme::PlayerControlScheme(int fwd, int bwd, int sLeft, int sRight,
                                         int lLeft, int lRight, int lUp, int lDown,
                                         int pBlock, int bBlock, int jmp,
                                         int swLeft, int swRight, int cmd, int crch)
        : forward(fwd), backward(bwd), strafeLeft(sLeft), strafeRight(sRight),
          lookLeft(lLeft), lookRight(lRight), lookUp(lUp), lookDown(lDown),
          placeBlock(pBlock), breakBlock(bBlock), jump(jmp),
          switchBlockLeft(swLeft), switchBlockRight(swRight),
          openCmd(cmd), crouch(crch) {
}

// PlayerController implementation
PlayerController::PlayerController(Player* player)
        : player(player), breakPlaceTimeout(0.f), openedCmd(false), flyTimer(-1.f) {

    bool isNew3ds = false;
    APT_CheckNew3DS(&isNew3ds);

    if (isNew3ds) {
        controlScheme = n3ds_default_scheme;
        this->player->autoJumpEnabled = false;
    } else {
        controlScheme = platform_default_scheme;
        this->player->autoJumpEnabled = true;
    }

    bool elementMissing = false;
    const char path[] = "sdmc:/craftus_redesigned/options.ini";

    if (access(path, F_OK) != -1) {
        ini_t* cfg = ini_load(path);
        char buffer[64];

        // Load control keys
        const char* controlKeys[] = {
                "forward", "backward", "strafeLeft", "strafeRight",
                "lookLeft", "lookRight", "lookUp", "lookDown",
                "placeBlock", "breakBlock", "jump", "switchBlockLeft",
                "switchBlockRight", "openCmd", "crouch"
        };

        int* controlSchemeKeys[] = {
                &controlScheme.forward, &controlScheme.backward,
                &controlScheme.strafeLeft, &controlScheme.strafeRight,
                &controlScheme.lookLeft, &controlScheme.lookRight,
                &controlScheme.lookUp, &controlScheme.lookDown,
                &controlScheme.placeBlock, &controlScheme.breakBlock,
                &controlScheme.jump, &controlScheme.switchBlockLeft,
                &controlScheme.switchBlockRight, &controlScheme.openCmd,
                &controlScheme.crouch
        };

        for (int k = 0; k < 15; k++) {
            if (ini_sget(cfg, "controls", controlKeys[k], "%s", buffer)) {
                for (int i = 0; i < 23; i++) {
                    if (!strcmp(platform_key_names[i], buffer)) {
                        *controlSchemeKeys[k] = i;
                        break;
                    }
                }
            } else {
                elementMissing = true;
            }
        }

        if (!ini_sget(cfg, "controls", "auto_jumping", "%d", &this->player->autoJumpEnabled))
            elementMissing = true;

        ini_free(cfg);
    } else {
        elementMissing = true;
    }

    if (elementMissing) {
        FILE* f = fopen(path, "w");
        fprintf(f, "[controls]\n");
        fprintf(f, "; The allowed key values are:\n; ");
        int j = 0;
        for (int i = 0; i < 23 - 1; i++) {
            fprintf(f, "%s, ", platform_key_names[i]);
            if (++j == 5) {
                j = 0;
                fprintf(f, "\n ; ");
            }
        }
        fprintf(f, "%s\n\n", platform_key_names[23 - 1]);

        // Write control keys
        fprintf(f, "forward=%s\n", platform_key_names[controlScheme.forward]);
        fprintf(f, "backward=%s\n", platform_key_names[controlScheme.backward]);
        fprintf(f, "strafeLeft=%s\n", platform_key_names[controlScheme.strafeLeft]);
        fprintf(f, "strafeRight=%s\n", platform_key_names[controlScheme.strafeRight]);
        fprintf(f, "lookLeft=%s\n", platform_key_names[controlScheme.lookLeft]);
        fprintf(f, "lookRight=%s\n", platform_key_names[controlScheme.lookRight]);
        fprintf(f, "lookUp=%s\n", platform_key_names[controlScheme.lookUp]);
        fprintf(f, "lookDown=%s\n", platform_key_names[controlScheme.lookDown]);
        fprintf(f, "placeBlock=%s\n", platform_key_names[controlScheme.placeBlock]);
        fprintf(f, "breakBlock=%s\n", platform_key_names[controlScheme.breakBlock]);
        fprintf(f, "jump=%s\n", platform_key_names[controlScheme.jump]);
        fprintf(f, "switchBlockLeft=%s\n", platform_key_names[controlScheme.switchBlockLeft]);
        fprintf(f, "switchBlockRight=%s\n", platform_key_names[controlScheme.switchBlockRight]);
        fprintf(f, "openCmd=%s\n", platform_key_names[controlScheme.openCmd]);
        fprintf(f, "crouch=%s\n", platform_key_names[controlScheme.crouch]);

        fprintf(f, "; 0 = disabled, 1 = enabled (default: 1 for O3DS, 0 for N3DS)\nautojump=%d\n",
                player->autoJumpEnabled);
        fclose(f);
    }
}

void PlayerController::Update(DebugUI* debugUi, InputData input, float dt) {
    Damage* dmg = nullptr;

    PlatformAgnosticInput agnosticInput = {0};
    convertPlatformInput(&input, agnosticInput.keys, agnosticInput.keysdown, agnosticInput.keysup);

    float jump = IsKeyDown(controlScheme.jump, &agnosticInput);
    float crouch = IsKeyDown(controlScheme.crouch, &agnosticInput);

    float forward = IsKeyDown(controlScheme.forward, &agnosticInput);
    float backward = IsKeyDown(controlScheme.backward, &agnosticInput);
    float strafeLeft = IsKeyDown(controlScheme.strafeLeft, &agnosticInput);
    float strafeRight = IsKeyDown(controlScheme.strafeRight, &agnosticInput);

    float3 forwardVec = f3_new(-sinf(player->yaw), 0.f, -cosf(player->yaw));
    float3 rightVec = f3_crs(forwardVec, f3_new(0, 1, 0));

    movement = f3_new(0, 0, 0);
    movement = f3_add(movement, f3_scl(forwardVec, forward));
    movement = f3_sub(movement, f3_scl(forwardVec, backward));
    movement = f3_add(movement, f3_scl(rightVec, strafeRight));
    movement = f3_sub(movement, f3_scl(rightVec, strafeLeft));

    if (player->flying) {
        movement = f3_add(movement, f3_new(0.f, jump, 0.f));
        movement = f3_sub(movement, f3_new(0.f, crouch, 0.f));
    }

    if (f3_magSqr(movement) > 0.f) {
        float speed = 4.3f * f3_mag(f3_new(-strafeLeft + strafeRight, -crouch + jump, -forward + backward));
        player->bobbing += speed * 1.5f * dt;
        movement = f3_scl(f3_nrm(movement), speed);
    }

    float lookLeft = IsKeyDown(controlScheme.lookLeft, &agnosticInput);
    float lookRight = IsKeyDown(controlScheme.lookRight, &agnosticInput);
    float lookUp = IsKeyDown(controlScheme.lookUp, &agnosticInput);
    float lookDown = IsKeyDown(controlScheme.lookDown, &agnosticInput);

    player->yaw += (lookLeft + -lookRight) * 160.f * DEG_TO_RAD * dt;
    player->pitch += (-lookDown + lookUp) * 160.f * DEG_TO_RAD * dt;
    player->pitch = CLAMP(player->pitch, -DEG_TO_RAD * 89.9f, DEG_TO_RAD * 89.9f);

    float placeBlock = IsKeyDown(controlScheme.placeBlock, &agnosticInput);
    float breakBlock = IsKeyDown(controlScheme.breakBlock, &agnosticInput);

    if (placeBlock > 0.f) player->PlaceBlock();
    if (breakBlock > 0.f) player->BreakBlock();

    if (jump > 0.f) player->Jump(mc::Vector3d(movement.x, movement.y, movement.z));

    bool releasedJump = WasKeyReleased(controlScheme.jump, &agnosticInput);
    if (flyTimer >= 0.f) {
        if (jump > 0.f) player->flying ^= true;
        flyTimer += dt;
        if (flyTimer > 0.25f) flyTimer = -1.f;
    } else if (releasedJump) {
        flyTimer = 0.f;
    }

    bool releasedCrouch = WasKeyReleased(controlScheme.crouch, &agnosticInput);
    player->crouching ^= !player->flying && releasedCrouch;

    bool switchBlockLeft = WasKeyPressed(controlScheme.switchBlockLeft, &agnosticInput);
    bool switchBlockRight = WasKeyPressed(controlScheme.switchBlockRight, &agnosticInput);

    if (switchBlockLeft && --player->quickSelectBarSlot == -1)
        player->quickSelectBarSlot = player->quickSelectBarSlots - 1;
    if (switchBlockRight && ++player->quickSelectBarSlot == player->quickSelectBarSlots)
        player->quickSelectBarSlot = 0;

    if (openedCmd) {
        dt = 0.f;
        openedCmd = false;
    }

    bool cmdLine = WasKeyPressed(controlScheme.openCmd, &agnosticInput);
    if (cmdLine) {
        CommandLine_Activate(player->world, player, debugUi);
        openedCmd = true;
    }

    player->Move(dt, mc::Vector3d(movement.x,movement.y,movement.z));
    player->Update(dmg);
}