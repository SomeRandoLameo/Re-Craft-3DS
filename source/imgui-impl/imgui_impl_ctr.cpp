// This Backend is heavily based on mtheall's ftpd citro3d and ctr
// implementation Link: https://github.com/mtheall/ftpd/blob/master/source/3ds/
#include <3ds.h>
#include <stdint.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <string>
#include <tuple>

#include "imgui_impl_ctr.h"
#include "../imgui/imgui_internal.h"

#define TICKS_PER_MSEC 268111.856

#ifdef IM_IMPL_CTR_NPI_ASSERT
#define NPI_ASSERT(expr)                                                  \
  if (!(expr)) {                                                          \
    std::ofstream logFile("sdmc:/assert_errors.txt", std::ios_base::app); \
    logFile << "Assertion failed in " << __FILE__ << " line " << __LINE__ \
            << ": " << #expr << std::endl;                                \
    logFile.close();                                                      \
    abort();                                                              \
  }
#else
#define NPI_ASSERT(expr) IM_ASSERT(expr)
#endif

static uint64_t last_time;
static std::string clipboard;

// Callbacks
const char *getClippBoardText(void *const userData_) {
  (void)userData_;
  return clipboard.c_str();
}

void setClipboardText(void *const userData_, char const *const text_) {
  (void)userData_;
  clipboard = text_;
}

void ProcessTouch(ImGuiIO &io) {
  if (hidKeysUp() & KEY_TOUCH) {
    io.AddMouseButtonEvent(0, false);
    return;
  }
  if (!(hidKeysHeld() & KEY_TOUCH)) {
    io.AddMousePosEvent(-10.0f, -10.0f);
    io.AddMouseButtonEvent(0, false);
    return;
  }
  touchPosition pos;
  hidTouchRead(&pos);
  io.AddMousePosEvent(pos.px + 40.0f, pos.py + 240.0f);
  io.AddMouseButtonEvent(0, true);
}

void ProcessInput(ImGuiIO &io) {
  auto const buttonMapping = {
      // clang-format off
	    std::make_pair (KEY_A,      ImGuiKey_GamepadFaceDown),  // A and B are swapped
	    std::make_pair (KEY_B,      ImGuiKey_GamepadFaceRight), // this is more intuitive
	    std::make_pair (KEY_X,      ImGuiKey_GamepadFaceUp),
	    std::make_pair (KEY_Y,      ImGuiKey_GamepadFaceLeft),
	    std::make_pair (KEY_L,      ImGuiKey_GamepadL1),
	    std::make_pair (KEY_ZL,     ImGuiKey_GamepadL2),
	    std::make_pair (KEY_R,      ImGuiKey_GamepadR1),
	    std::make_pair (KEY_ZR,     ImGuiKey_GamepadR2),
	    std::make_pair (KEY_DUP,    ImGuiKey_GamepadDpadUp),
	    std::make_pair (KEY_DRIGHT, ImGuiKey_GamepadDpadRight),
	    std::make_pair (KEY_DDOWN,  ImGuiKey_GamepadDpadDown),
	    std::make_pair (KEY_DLEFT,  ImGuiKey_GamepadDpadLeft),
      std::make_pair (KEY_SELECT, ImGuiKey_GamepadBack),
      std::make_pair (KEY_START,  ImGuiKey_GamepadStart),
      // clang-format on
  };
  for (auto const &[in, out] : buttonMapping) {
    if (hidKeysUp() & in)
      io.AddKeyEvent(out, false);
    else if (hidKeysDown() & in)
      io.AddKeyEvent(out, true);
  }
  circlePosition cpad;
  auto const analogMapping = {
      // clang-format off
	    std::make_tuple (std::ref (cpad.dx), ImGuiKey_GamepadLStickLeft,  -0.3f, -0.9f),
	    std::make_tuple (std::ref (cpad.dx), ImGuiKey_GamepadLStickRight, +0.3f, +0.9f),
	    std::make_tuple (std::ref (cpad.dy), ImGuiKey_GamepadLStickUp,    +0.3f, +0.9f),
	    std::make_tuple (std::ref (cpad.dy), ImGuiKey_GamepadLStickDown,  -0.3f, -0.9f),
      // clang-format on
  };
  hidCircleRead(&cpad);
  for (auto const &[in, out, min, max] : analogMapping) {
    auto const value =
        std::clamp((in / 156.0f - min) / (max - min), 0.0f, 1.0f);
    io.AddKeyAnalogEvent(out, value > 0.1f, value);
  }
  circlePosition cstick;
  auto const analogCMapping = {
      // clang-format off
	    std::make_tuple (std::ref (cstick.dx), ImGuiKey_GamepadRStickLeft,  -0.3f, -0.9f),
	    std::make_tuple (std::ref (cstick.dx), ImGuiKey_GamepadRStickRight, +0.3f, +0.9f),
	    std::make_tuple (std::ref (cstick.dy), ImGuiKey_GamepadRStickUp,    +0.3f, +0.9f),
	    std::make_tuple (std::ref (cstick.dy), ImGuiKey_GamepadRStickDown,  -0.3f, -0.9f),
      // clang-format on
  };
  hidCstickRead(&cstick);
  for (auto const &[in, out, min, max] : analogCMapping) {
    auto const value =
        std::clamp((in / 156.0f - min) / (max - min), 0.0f, 1.0f);
    io.AddKeyAnalogEvent(out, value > 0.1f, value);
  }
}

void ProcessKeyboard(ImGuiIO &io) {
  static enum {
    INACTIVE,
    KEYBOARD,
    CLEARED,
  } state = INACTIVE;

  switch (state) {
    case INACTIVE: {
      if (!io.WantTextInput) return;
      auto &textState = ImGui::GetCurrentContext()->InputTextState;
      SwkbdState kbd;
      swkbdInit(&kbd, SWKBD_TYPE_NORMAL, 2, -1);
      swkbdSetButton(&kbd, SWKBD_BUTTON_LEFT, "Cancel", false);
      swkbdSetButton(&kbd, SWKBD_BUTTON_RIGHT, "OK", true);
      swkbdSetInitialText(&kbd, std::string(textState.InitialTextA.Data,
                                            textState.InitialTextA.Size)
                                    .c_str());
      if (textState.Flags & ImGuiInputTextFlags_Password)
        swkbdSetPasswordMode(&kbd, SWKBD_PASSWORD_HIDE_DELAY);
      char buffer[32] = {0};
      auto const button = swkbdInputText(&kbd, buffer, sizeof(buffer));
      if (button == SWKBD_BUTTON_RIGHT) io.AddInputCharactersUTF8(buffer);
      state = KEYBOARD;
      break;
    }
    case KEYBOARD:
      // need to skip a frame for active id to really be cleared
      ImGui::ClearActiveID();
      state = CLEARED;
      break;
    case CLEARED:
      state = INACTIVE;
      break;
  }
}

// ImGui Impl Functions
IMGUI_IMPL_API bool ImGui_ImplCtr_Init() {
  auto &io = ImGui::GetIO();

  // Configuration
  io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // Backend
  io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
  io.BackendPlatformName = "imgui_impl_ctr";

  // Disable Cursor
  io.MouseDrawCursor = false;

  // Callbacks
  io.SetClipboardTextFn = setClipboardText;
  io.GetClipboardTextFn = getClippBoardText;
  io.ClipboardUserData = nullptr;

  return true;
}

IMGUI_IMPL_API void ImGui_ImplCtr_Shutdown() {
  // NO CODE FOR THIS FUNC??
}

IMGUI_IMPL_API void ImGui_ImplCtr_NewFrame() {
  auto &io = ImGui::GetIO();
  NPI_ASSERT(io.Fonts->IsBuilt() &&
             "Font atlas not built! It is generally built by the renderer "
             "back-end. Missing call to renderer _NewFrame() function?");

  uint64_t currentTime = svcGetSystemTick();
  io.DeltaTime = ((float)(currentTime / (float)TICKS_PER_MSEC) -
                  (float)(last_time / (float)TICKS_PER_MSEC)) /
                 1000.f;
  last_time = currentTime;
  ProcessInput(io);
  ProcessKeyboard(io);
  ProcessTouch(io);
}
