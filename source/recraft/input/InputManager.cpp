#include "input/InputManager.hpp"

InputManager* InputManager::instance = new InputManager();

InputManager::InputManager() {
    APT_CheckNew3DS(&m_isNewModel);
    loadKeys();
}

void InputManager::loadKeys() {
    for (u32 i = 0; i < InputAction::INP_COUNT; ++i) {
        InputEntry& act = m_keyset.list[i];
        if (m_isNewModel) {
            if (act.defkeyNw == KEY_NONE) {
                continue;
            }

            act.key = act.defkeyNw; // TODO: Implement config via "settings"
        } else {
            if (act.defkeyOg == KEY_NONE) {
                continue;
            }

            act.key = act.defkeyOg;
        }
    }
}

void InputManager::poll() {
    m_touchPrev = m_touch;
    hidScanInput();
    m_keysHeld = hidKeysHeld();
    m_keysDown = hidKeysDown();
    m_keysUp   = hidKeysUp();
    hidCircleRead(&m_pad);
    hidCstickRead(&m_stick);
    hidTouchRead(&m_touch);

    for (u32 i = 0; i < InputAction::INP_COUNT; ++i) {
        InputEntry& act = m_keyset.list[i];

        if (act.key == KEY_NONE) {
            continue;
        }

        u32 keys = act.isHold ? m_keysHeld : m_keysDown;

        act.statePress   = keys & act.key;
        act.stateRelease = m_keysUp & act.key;
    }
}
