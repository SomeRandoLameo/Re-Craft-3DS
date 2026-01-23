#include "entity/PlayerController.hpp"

#include "misc/NumberUtils.hpp"

#include <unistd.h>

extern "C" {
#include <ini/ini.h>
}

#include <3ds.h>

const char *platform_key_names[23] = {
    "Not Set",     "A",         "B",        "X",          "Y",
    "L",           "R",         "Start",    "Select",     "DUp",
    "DDown",       "DLeft",     "DRight",   "CircUp",     "CircDown",
    "CircLeft",    "CircRight", "CStickUp", "CStickDown", "CStickLeft",
    "CStickRight", "ZL",        "ZR"};

enum {
    K3DS_Undefined = 0,
    K3DS_A,
    K3DS_B,
    K3DS_X,
    K3DS_Y,
    K3DS_L,
    K3DS_R,
    K3DS_START,
    K3DS_SELECT,
    K3DS_DUP,
    K3DS_DDOWN,
    K3DS_DLEFT,
    K3DS_DRIGHT,
    K3DS_CPAD_UP,
    K3DS_CPAD_DOWN,
    K3DS_CPAD_LEFT,
    K3DS_CPAD_RIGHT,
    K3DS_CSTICK_UP,
    K3DS_CSTICK_DOWN,
    K3DS_CSTICK_LEFT,
    K3DS_CSTICK_RIGHT,
    K3DS_ZL,
    K3DS_ZR
};

// Default control schemes
static const PlayerControlScheme
    platform_default_scheme(K3DS_CPAD_UP,    // forward
                            K3DS_CPAD_DOWN,  // backward
                            K3DS_CPAD_LEFT,  // strafeLeft
                            K3DS_CPAD_RIGHT, // strafeRight
                            K3DS_Y,          // lookLeft
                            K3DS_A,          // lookRight
                            K3DS_X,          // lookUp
                            K3DS_B,          // lookDown
                            K3DS_L,          // placeBlock
                            K3DS_R,          // breakBlock
                            K3DS_DUP,        // jump
                            K3DS_DLEFT,      // switchBlockLeft
                            K3DS_DRIGHT,     // switchBlockRight
                            K3DS_SELECT,     // openCmd
                            K3DS_DDOWN       // crouch
    );

static const PlayerControlScheme
    n3ds_default_scheme(K3DS_CPAD_UP,      // forward
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



void convertPlatformInput(InputData *input, float ctrls[], bool keysdown[], bool keysup[]) {
    ctrls[0] = 0.f;
    keysdown[0] = 0;
    keysup[0] = 0;

    // Binary keys
    const struct {
        int index;
        u32 key;
    } binaryKeys[] = {
        {K3DS_A, KEY_A},         {K3DS_B, KEY_B},
        {K3DS_X, KEY_X},         {K3DS_Y, KEY_Y},
        {K3DS_L, KEY_L},         {K3DS_R, KEY_R},
        {K3DS_START, KEY_START}, {K3DS_SELECT, KEY_SELECT},
        {K3DS_DUP, KEY_DUP},     {K3DS_DDOWN, KEY_DDOWN},
        {K3DS_DLEFT, KEY_DLEFT}, {K3DS_DRIGHT, KEY_DRIGHT},
        {K3DS_ZL, KEY_ZL},       {K3DS_ZR, KEY_ZR}
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

    for (auto circlePadKey : circlePadKeys) {
        int idx = circlePadKey.index;
        float val = circlePadKey.value;
        u32 key = circlePadKey.key;
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

    for (auto cstickKey : cstickKeys) {
        int idx = cstickKey.index;
        float val = cstickKey.value;
        u32 key = cstickKey.key;
        ctrls[idx] = ABS(val) * (float)((input->keysdown & key) || (input->keysheld & key));
        keysdown[idx] = input->keysdown & key;
        keysup[idx] = input->keysup & key;
    }
}

// Utility functions


PlayerControlScheme::PlayerControlScheme(int fwd, int bwd, int sLeft,
                                         int sRight, int lLeft, int lRight,
                                         int lUp, int lDown, int pBlock,
                                         int bBlock, int jmp, int swLeft,
                                         int swRight, int cmd, int crch)
    : forward(fwd), backward(bwd), strafeLeft(sLeft), strafeRight(sRight),
      lookLeft(lLeft), lookRight(lRight), lookUp(lUp), lookDown(lDown),
      placeBlock(pBlock), breakBlock(bBlock), jump(jmp),
      switchBlockLeft(swLeft), switchBlockRight(swRight), openCmd(cmd),
      crouch(crch) {}

// PlayerController implementation
PlayerController::PlayerController(Player *player) : player(player) {
    bool isNew3ds = false;
    APT_CheckNew3DS(&isNew3ds);

    if (isNew3ds) {
        m_controlScheme = n3ds_default_scheme;
        this->player->autoJumpEnabled = false;
    } else {
        m_controlScheme = platform_default_scheme;
        this->player->autoJumpEnabled = true;
    }

    bool elementMissing = false;
    const char path[] = "sdmc:/" SAVE_DIR "/options.ini";

    if (access(path, F_OK) != -1) {
        ini_t *cfg = ini_load(path);
        char buffer[64];

        // Load control keys
        const char *controlKeys[] = {
            "forward",          "backward",   "strafeLeft", "strafeRight",
            "lookLeft",         "lookRight",  "lookUp",     "lookDown",
            "placeBlock",       "breakBlock", "jump",       "switchBlockLeft",
            "switchBlockRight", "openCmd",    "crouch"};

        int *controlSchemeKeys[] = {
            &m_controlScheme.forward,
            &m_controlScheme.backward,
            &m_controlScheme.strafeLeft,
            &m_controlScheme.strafeRight,
            &m_controlScheme.lookLeft,
            &m_controlScheme.lookRight,
            &m_controlScheme.lookUp,
            &m_controlScheme.lookDown,
            &m_controlScheme.placeBlock,
            &m_controlScheme.breakBlock,
            &m_controlScheme.jump,
            &m_controlScheme.switchBlockLeft,
            &m_controlScheme.switchBlockRight,
            &m_controlScheme.openCmd,
            &m_controlScheme.crouch
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

        if (!ini_sget(cfg, "controls", "auto_jumping", "%d", &this->player->autoJumpEnabled)){
            elementMissing = true;
        }

        ini_free(cfg);
    } else {
        elementMissing = true;
    }

    if (elementMissing) {
        FILE *f = fopen(path, "w");
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
        fprintf(f, "%s\n\n",        platform_key_names[23 - 1]);

        // Write control keys
        fprintf(f, "forward=%s\n",          platform_key_names[m_controlScheme.forward]);
        fprintf(f, "backward=%s\n",         platform_key_names[m_controlScheme.backward]);
        fprintf(f, "strafeLeft=%s\n",       platform_key_names[m_controlScheme.strafeLeft]);
        fprintf(f, "strafeRight=%s\n",      platform_key_names[m_controlScheme.strafeRight]);
        fprintf(f, "lookLeft=%s\n",         platform_key_names[m_controlScheme.lookLeft]);
        fprintf(f, "lookRight=%s\n",        platform_key_names[m_controlScheme.lookRight]);
        fprintf(f, "lookUp=%s\n",           platform_key_names[m_controlScheme.lookUp]);
        fprintf(f, "lookDown=%s\n",         platform_key_names[m_controlScheme.lookDown]);
        fprintf(f, "placeBlock=%s\n",       platform_key_names[m_controlScheme.placeBlock]);
        fprintf(f, "breakBlock=%s\n",       platform_key_names[m_controlScheme.breakBlock]);
        fprintf(f, "jump=%s\n",             platform_key_names[m_controlScheme.jump]);
        fprintf(f, "switchBlockLeft=%s\n",  platform_key_names[m_controlScheme.switchBlockLeft]);
        fprintf(f, "switchBlockRight=%s\n", platform_key_names[m_controlScheme.switchBlockRight]);
        fprintf(f, "openCmd=%s\n",          platform_key_names[m_controlScheme.openCmd]);
        fprintf(f, "crouch=%s\n",           platform_key_names[m_controlScheme.crouch]);

        fprintf(f,
                "; 0 = disabled, 1 = enabled (default: 1 for O3DS, 0 for "
                "N3DS)\nautojump=%d\n",
                player->autoJumpEnabled);
        fclose(f);
    }
}
