// dear imgui: Platform Backend for CTR (Nintendo3DS 'CTR' and NEW3DS 'SNAKE')
// This needs to be used along with a Renderer (e.g. Citro3D, 'Software, PicaGL
// my be in future')

// Implemented features:
// [X] Platform: Clipboard support (just for imgui (you are not abele to use it
// out of it)). [X] Platform: Touch support. Registers Touch inputs from Bottom
// Screen [X] Platform: Gamepad support. All Keys are Set also ZL and ZR +
// CStick for N3DS (May be User Keamapping in future) [X] Platform: Currently
// uses SWKBD for Keyboard input stuff but i will replace it trough NpiKBD-Lite
// when its finished

// You can use unmodified imgui_impl_* files in your project. See examples/
// folder for examples of using this. Prefer including the entire imgui/
// repository into your project (either as a copy or as a submodule), and only
// build the backends you need. If you are new to Dear ImGui, read documentation
// from the docs/ folder + read the top of imgui.cpp. Read online:
// https://github.com/ocornut/imgui/tree/master/docs

// This Backend is heavily based on mtheall's ftpd citro3d and ctr
// implementation Link: https://github.com/mtheall/ftpd/blob/master/source/3ds/
#pragma once

#include "../imgui/imgui.h"  // IMGUI_IMPL_API

/// NPI_ASSERT Allows writing problems into a logfile
/// IF IT'S DISABLED IT USES IM_ASSERT instead
#ifdef IMGUI_IMPL_CTR_USE_NPI_ASSERT
#define IM_IMPL_CTR_NPI_ASSERT
#endif

/// @brief Initialisize ImGui for 3ds
/// @return Sucess or Not
IMGUI_IMPL_API bool ImGui_ImplCtr_Init();
/// @brief This function does Nothing but its implemented for
/// later changes maybe
IMGUI_IMPL_API void ImGui_ImplCtr_Shutdown();
/// @brief This Function is for Calculating DeltaTime
IMGUI_IMPL_API void ImGui_ImplCtr_NewFrame();