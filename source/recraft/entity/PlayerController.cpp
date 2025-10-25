#include "PlayerController.h"

#include "../misc/CommandLine.h"
#include "../misc/NumberUtils.h"

#include <unistd.h>
//#include "../misc/Sound.h"

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

const PlayerControlScheme platform_default_scheme = {
    .forward = K3DS_CPAD_UP,
    .backward = K3DS_CPAD_DOWN,
    .strafeLeft = K3DS_CPAD_LEFT,
    .strafeRight = K3DS_CPAD_RIGHT,
    .lookLeft = K3DS_Y,
    .lookRight = K3DS_A,
    .lookUp = K3DS_X,
    .lookDown = K3DS_B,
    .placeBlock = K3DS_L,
    .breakBlock = K3DS_R,
    .jump = K3DS_DUP,
    .switchBlockLeft = K3DS_DLEFT,
    .switchBlockRight = K3DS_DRIGHT,
    .openCmd = K3DS_SELECT,
    .crouch = K3DS_DDOWN
};

const PlayerControlScheme n3ds_default_scheme = {
    .forward = K3DS_CPAD_UP,
    .backward = K3DS_CPAD_DOWN,
    .strafeLeft = K3DS_CPAD_LEFT,
    .strafeRight = K3DS_CPAD_RIGHT,
    .lookLeft = K3DS_CSTICK_LEFT,
    .lookRight = K3DS_CSTICK_RIGHT,
    .lookUp = K3DS_CSTICK_UP,
    .lookDown = K3DS_CSTICK_DOWN,
    .placeBlock = K3DS_ZL,
    .breakBlock = K3DS_ZR,
    // Fixed invalid '&&' expressions (pick primary buttons instead)
    .jump = K3DS_A,
    .switchBlockLeft = K3DS_L,
    .switchBlockRight = K3DS_R,
    .openCmd = K3DS_SELECT,
    .crouch = K3DS_Y
};

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

typedef struct {
    float keys[23];
    bool keysup[23];
    bool keysdown[23];
} PlatformAgnosticInput;


// Utility functions
static inline float IsKeyDown(KeyCombo combo, PlatformAgnosticInput* input) { return input->keys[combo]; }
static inline bool WasKeyReleased(KeyCombo combo, PlatformAgnosticInput* input) { return input->keysup[combo]; }
static inline bool WasKeyPressed(KeyCombo combo, PlatformAgnosticInput* input) { return input->keysdown[combo]; }

void PlayerController_Init(PlayerController* ctrl, Player* player) {
    ctrl->breakPlaceTimeout = 0.f;
    ctrl->player = player;

    bool isNew3ds = false;
    APT_CheckNew3DS(&isNew3ds);
    if (isNew3ds) {
        ctrl->controlScheme = n3ds_default_scheme;
        ctrl->player->autoJumpEnabled = false;
    } else {
        ctrl->controlScheme = platform_default_scheme;
        ctrl->player->autoJumpEnabled = true;
    }

    ctrl->openedCmd = false;
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
                &ctrl->controlScheme.forward, &ctrl->controlScheme.backward,
                &ctrl->controlScheme.strafeLeft, &ctrl->controlScheme.strafeRight,
                &ctrl->controlScheme.lookLeft, &ctrl->controlScheme.lookRight,
                &ctrl->controlScheme.lookUp, &ctrl->controlScheme.lookDown,
                &ctrl->controlScheme.placeBlock, &ctrl->controlScheme.breakBlock,
                &ctrl->controlScheme.jump, &ctrl->controlScheme.switchBlockLeft,
                &ctrl->controlScheme.switchBlockRight, &ctrl->controlScheme.openCmd,
                &ctrl->controlScheme.crouch
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

        if (!ini_sget(cfg, "controls", "auto_jumping", "%d", &ctrl->player->autoJumpEnabled))
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
        fprintf(f, "forward=%s\n", platform_key_names[ctrl->controlScheme.forward]);
        fprintf(f, "backward=%s\n", platform_key_names[ctrl->controlScheme.backward]);
        fprintf(f, "strafeLeft=%s\n", platform_key_names[ctrl->controlScheme.strafeLeft]);
        fprintf(f, "strafeRight=%s\n", platform_key_names[ctrl->controlScheme.strafeRight]);
        fprintf(f, "lookLeft=%s\n", platform_key_names[ctrl->controlScheme.lookLeft]);
        fprintf(f, "lookRight=%s\n", platform_key_names[ctrl->controlScheme.lookRight]);
        fprintf(f, "lookUp=%s\n", platform_key_names[ctrl->controlScheme.lookUp]);
        fprintf(f, "lookDown=%s\n", platform_key_names[ctrl->controlScheme.lookDown]);
        fprintf(f, "placeBlock=%s\n", platform_key_names[ctrl->controlScheme.placeBlock]);
        fprintf(f, "breakBlock=%s\n", platform_key_names[ctrl->controlScheme.breakBlock]);
        fprintf(f, "jump=%s\n", platform_key_names[ctrl->controlScheme.jump]);
        fprintf(f, "switchBlockLeft=%s\n", platform_key_names[ctrl->controlScheme.switchBlockLeft]);
        fprintf(f, "switchBlockRight=%s\n", platform_key_names[ctrl->controlScheme.switchBlockRight]);
        fprintf(f, "openCmd=%s\n", platform_key_names[ctrl->controlScheme.openCmd]);
        fprintf(f, "crouch=%s\n", platform_key_names[ctrl->controlScheme.crouch]);

        fprintf(f, "; 0 = disabled, 1 = enabled (default: 1 for O3DS, 0 for N3DS)\nautojump=%d\n",
                player->autoJumpEnabled);
        fclose(f);
    }

    ctrl->flyTimer = -1.f;
}

void PlayerController_Update(PlayerController* ctrl, DebugUI* debugUi,/* Sound* sound, */InputData input, float dt) {
    Player* player = ctrl->player;

    Damage* dmg = NULL; // setting to NULL properly

    PlatformAgnosticInput agnosticInput = {0};


    convertPlatformInput(&input, agnosticInput.keys, agnosticInput.keysdown, agnosticInput.keysup);

    float jump = IsKeyDown(ctrl->controlScheme.jump, &agnosticInput);
    float crouch = IsKeyDown(ctrl->controlScheme.crouch, &agnosticInput);

    float forward = IsKeyDown(ctrl->controlScheme.forward, &agnosticInput);
    float backward = IsKeyDown(ctrl->controlScheme.backward, &agnosticInput);
    float strafeLeft = IsKeyDown(ctrl->controlScheme.strafeLeft, &agnosticInput);
    float strafeRight = IsKeyDown(ctrl->controlScheme.strafeRight, &agnosticInput);

    float3 forwardVec = f3_new(-sinf(player->yaw), 0.f, -cosf(player->yaw));
    float3 rightVec = f3_crs(forwardVec, f3_new(0, 1, 0));

    float3 movement = f3_new(0, 0, 0);
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

    float lookLeft = IsKeyDown(ctrl->controlScheme.lookLeft, &agnosticInput);
    float lookRight = IsKeyDown(ctrl->controlScheme.lookRight, &agnosticInput);
    float lookUp = IsKeyDown(ctrl->controlScheme.lookUp, &agnosticInput);
    float lookDown = IsKeyDown(ctrl->controlScheme.lookDown, &agnosticInput);

    player->yaw += (lookLeft + -lookRight) * 160.f * DEG_TO_RAD * dt;
    player->pitch += (-lookDown + lookUp) * 160.f * DEG_TO_RAD * dt;
    player->pitch = CLAMP(player->pitch, -DEG_TO_RAD * 89.9f, DEG_TO_RAD * 89.9f);

    float placeBlock = IsKeyDown(ctrl->controlScheme.placeBlock, &agnosticInput);
    float breakBlock = IsKeyDown(ctrl->controlScheme.breakBlock, &agnosticInput);
    if (placeBlock > 0.f) player->PlaceBlock();/*, sound*/
    if (breakBlock > 0.f) player->BreakBlock();

    if (jump > 0.f) player->Jump(movement);

    bool releasedJump = WasKeyReleased(ctrl->controlScheme.jump, &agnosticInput);
    if (ctrl->flyTimer >= 0.f) {
        if (jump > 0.f) player->flying ^= true;
        ctrl->flyTimer += dt;
        if (ctrl->flyTimer > 0.25f) ctrl->flyTimer = -1.f;
    } else if (releasedJump) {
        ctrl->flyTimer = 0.f;
    }

    bool releasedCrouch = WasKeyReleased(ctrl->controlScheme.crouch, &agnosticInput);
    player->crouching ^= !player->flying && releasedCrouch;

    bool switchBlockLeft = WasKeyPressed(ctrl->controlScheme.switchBlockLeft, &agnosticInput);
    bool switchBlockRight = WasKeyPressed(ctrl->controlScheme.switchBlockRight, &agnosticInput);
    if (switchBlockLeft && --player->quickSelectBarSlot == -1) player->quickSelectBarSlot = player->quickSelectBarSlots - 1;
    if (switchBlockRight && ++player->quickSelectBarSlot == player->quickSelectBarSlots) player->quickSelectBarSlot = 0;

    if (ctrl->openedCmd) {
        dt = 0.f;
        ctrl->openedCmd = false;
    }

    float cmdLine = WasKeyPressed(ctrl->controlScheme.openCmd, &agnosticInput);
    if (cmdLine) {
        CommandLine_Activate(player->world, player,debugUi);
        ctrl->openedCmd = true;
    }

    player->Move( dt, movement);


    player->Update(/*sound, */dmg);
}
