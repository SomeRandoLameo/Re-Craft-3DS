#include "keyboard.hpp"
#include <3ds.h>
#include <array>

std::string getKeyboardInput(const std::string& prompt) {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY, 0, 0);
    swkbdSetHintText(&swkbd, prompt.c_str());

    std::array<char, 256> buffer{};
    swkbdInputText(&swkbd, buffer.data(), buffer.size());

    return std::string(buffer.data());
}
