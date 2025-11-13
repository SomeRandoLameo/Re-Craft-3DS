// dear imgui: Renderer Backend for Citro3D
// This needs to be used along with a Platform Backend (e.g. ctr)

// Implemented features:
//  [X] Renderer: User texture binding. Use the adress of C3D_Tex as
//  ImTextureID. Read the FAQ about ImTextureID!

// You can use unmodified imgui_impl_* files in your project. See examples/
// folder for examples of using this. Prefer including the entire imgui/
// repository into your project (either as a copy or as a submodule), and only
// build the backends you need. If you are new to Dear ImGui, read documentation
// from the docs/ folder + read the top of imgui.cpp. Read online:
// https://github.com/ocornut/imgui/tree/master/docs

// This Backend is heavily based on mtheall's ftpd citro3d and ctr
// implementation Link: https://github.com/mtheall/ftpd/blob/master/source/3ds/
#pragma once
#include "../imgui/imgui.h"

/// NPI_ASSERT Allows writing problems into a logfile
/// IF IT'S DISABLED IT USES IM_ASSERT instead
#ifdef IMGUI_IMPL_CITRO3D_USE_NPI_ASSERT
#define IM_IMPL_C3D_NPI_ASSERT
#endif

/// @brief Initialisize Citro3D Backend (System Font is Standart and only
/// possible yet)
/// @param load_sysfont this bool is currently useless cause fonthandling is not
/// done yet
/// @return Success or Not
IMGUI_IMPL_API bool ImGui_ImplCitro3D_Init(bool load_sysfont = false);
/// @brief Deinitialisize ImGui Citro3D Backend
IMGUI_IMPL_API void ImGui_ImplCitro3D_Shutdown();
/// @brief Currently only Context Ceck
IMGUI_IMPL_API void ImGui_ImplCitro3D_NewFrame();
/// @brief Render The Data to the Screen
/// @param draw_data 'ImGui::GetDrawData()'
/// @param t_top Top Screen Target
/// @param t_bot Bottom Screen Target
IMGUI_IMPL_API void ImGui_ImplCitro3D_RenderDrawData(ImDrawData* draw_data,
                                                     void* t_top, void* t_bot);