#pragma once

#include <3ds.h>

#define KEY_NONE 0

struct InputEntry {
    // read only
    char showName[64];
    char confName[32];
    u32 defkeyOg;
    u32 defkeyNw;
    bool isHold = false;
    // read write
    u32 key = KEY_NONE;
    bool statePress = false;
    bool stateRelease = false;
};

enum InputAction {
    INP_ATTACK,
    INP_USE,
    INP_MOVE_FORWARD,
    INP_MOVE_BACK,
    INP_MOVE_LEFT,
    INP_MOVE_RIGHT,
    INP_LOOK_UP,
    INP_LOOK_DOWN,
    INP_LOOK_LEFT,
    INP_LOOK_RIGHT,
    INP_JUMP,
    INP_SNEAK,
    INP_DROP,

    INP_INVENTORY,
    INP_CHAT,
    INP_PLAYERLIST,
    INP_TOGGLE_PERSPECTIVE,
    INP_SWAP_HANDS,
    INP_HOTBAR_LEFT,
    INP_HOTBAR_RIGHT,
    INP_PAUSE,
    INP_MENU_UP,
    INP_MENU_DOWN,
    INP_MENU_LEFT,
    INP_MENU_RIGHT,
    INP_MENU_CONFIRM,
    INP_MENU_BACK,
    INP_COUNT
};

struct InputSet {

    InputEntry list[INP_COUNT];

    InputSet() : list{} {
        list[INP_ATTACK] = {"attack", "Attack", KEY_R, KEY_R, true};
        list[INP_USE] = {"use", "Use", KEY_L, KEY_L, true};

        list[INP_MOVE_FORWARD] = {"moveforward", "Move Forward", KEY_CPAD_UP, KEY_CPAD_UP, true};
        list[INP_MOVE_BACK] = {"moveback", "Move Back", KEY_CPAD_DOWN, KEY_CPAD_DOWN, true};
        list[INP_MOVE_LEFT] = {"moveleft", "Move Left", KEY_CPAD_LEFT, KEY_CPAD_LEFT, true};
        list[INP_MOVE_RIGHT] = {"moveright", "Move Right", KEY_CPAD_RIGHT, KEY_CPAD_RIGHT, true};

        list[INP_LOOK_UP] = {"lookup", "Look Up", KEY_DUP, KEY_CSTICK_UP, true};
        list[INP_LOOK_DOWN] = {"lookdown", "Look Down", KEY_DDOWN, KEY_CSTICK_DOWN, true};
        list[INP_LOOK_LEFT] = {"lookleft", "Look Left", KEY_DLEFT, KEY_CSTICK_LEFT, true};
        list[INP_LOOK_RIGHT] = {"lookright", "Look Right", KEY_DRIGHT, KEY_CSTICK_RIGHT, true};

        list[INP_JUMP] = {"jump", "Jump", KEY_A, KEY_A};
        list[INP_SNEAK] = {"sneak", "Sneak", KEY_Y, KEY_Y};
        list[INP_DROP] = {"drop", "Drop Item", KEY_B, KEY_B};
        list[INP_INVENTORY] = {"inventory", "Open Inventory", KEY_X, KEY_X};
        list[INP_CHAT] = {"chat", "Open Chat", KEY_NONE, KEY_DLEFT};
        list[INP_PLAYERLIST] = {"playerlist", "Show Playerlist", KEY_NONE, KEY_DUP};
        list[INP_TOGGLE_PERSPECTIVE] = {"togglePerspective", "Toggle Perspective", KEY_NONE, KEY_DDOWN};

        list[INP_SWAP_HANDS] = {"swapHands", "Swap Hands", KEY_NONE, KEY_DRIGHT};
        list[INP_HOTBAR_LEFT] = {"hotbarleft", "Hotbar Left", KEY_NONE, KEY_ZL};
        list[INP_HOTBAR_RIGHT] = {"hotbarright", "Hotbar Right", KEY_NONE, KEY_ZR};

        list[INP_PAUSE] = {"pause", "Pause Menu", KEY_START, KEY_START};

        list[INP_MENU_UP] = {"menuup", "Menu Up", KEY_UP, KEY_UP, true};
        list[INP_MENU_DOWN] = {"menudown", "Menu Down", KEY_DOWN, KEY_DOWN, true};
        list[INP_MENU_LEFT] = {"menuleft", "Menu Left", KEY_LEFT, KEY_LEFT, true};
        list[INP_MENU_RIGHT] = {"menuright", "Menu Right", KEY_RIGHT, KEY_RIGHT, true};
        list[INP_MENU_CONFIRM] = {"menuconfirm", "Menu Confirm", KEY_A, KEY_A};
        list[INP_MENU_BACK] = {"menuback", "Menu Back", KEY_B, KEY_B};
    }
};

