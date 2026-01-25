#pragma once

#include "input/InputSet.hpp"

class ReCraftCore;
class InputManager {
    static InputManager* instance;

    bool m_isNewModel;

    u32            m_keysUp, m_keysHeld, m_keysDown;
    InputSet       m_keyset = InputSet(); // Set of actions w. keys
    circlePosition m_pad; // Circle Pad
    circlePosition m_stick; // C Stick (new 3DS)
    touchPosition  m_touch;
    touchPosition  m_touchPrev;

private:
    InputManager();

    void loadKeys();

public:
    static InputManager* getInstance() { return instance; }

    void poll();

    inline bool           keyDown(u32 key) { return m_keysDown & key; }
    inline bool           keyHeld(u32 key) { return m_keysHeld & key; }
    inline bool           keyUp(u32 key) { return m_keysUp & key; }
    inline bool           actPress(InputAction action) { return m_keyset.list[action].statePress; }
    inline bool           actRel(InputAction action) { return m_keyset.list[action].stateRelease; }
    inline u32            getKey(InputAction action) { return m_keyset.list[action].key; }
    inline circlePosition getPad() { return m_pad; }
    inline circlePosition getStick() { return m_stick; }
    inline touchPosition  getTouch() { return m_touch; }
    inline touchPosition  getTouchPrev() { return m_touchPrev; }
};

namespace Input {
    static inline InputManager* getMgr() {
        return InputManager::getInstance();
    }
    static inline bool isPressed(InputAction action) {
        return getMgr()->actPress(action);
    }
    static inline bool isReleased(InputAction action) {
        return getMgr()->actRel(action);
    }
    static inline bool isKeyDown(u32 key) {
        return getMgr()->keyDown(key);
    }
    static inline bool isKeyHeld(u32 key) {
        return getMgr()->keyHeld(key);
    }
    static inline bool isKeyUp(u32 key) {
        return getMgr()->keyUp(key);
    }
    static inline u32 getKey(InputAction action) {
        return getMgr()->getKey(action);
    }
    static inline circlePosition getPad() {
        return getMgr()->getPad();
    }
    static inline circlePosition getStick() {
        return getMgr()->getStick();
    }
    static inline touchPosition getTouch() {
        return getMgr()->getTouch();
    }
    static inline touchPosition getTouchPrev() {
        return getMgr()->getTouchPrev();
    }
} // namespace Input
