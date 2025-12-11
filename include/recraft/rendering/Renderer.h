#pragma once

#include "../GameStates.h"
#include "../entity/Player.h"
#include "../gui/DebugUI.h"
#include "../world/CT_World.h"
#include "../world/WorkQueue.h"
#include "WorldRenderer.h"
#include "gui/ImGuiManager.h"

#include <citro3d.h>

// TODO: Organize
#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

// Enable Npi Assert to write Errors to file
#define IMGUI_IMPL_CITRO3D_USE_NPI_ASSERT
#define IMGUI_IMPL_CIR_USE_NPI_ASSERT

#include "imgui.h"
#include "imgui_impl_citro3d.h"
#include "imgui_impl_ctr.h"

class ReCraftCore;

class Renderer {
public:
  Renderer(World *world_, Player *player_, WorkQueue *queue);
  ~Renderer();

  void Render(DebugUI *debugUi);

  C3D_RenderTarget *GetTopTarget() const { return renderTargets[0]; }
  C3D_RenderTarget *GetBottomTarget() const { return lowerScreen; }

private:
  Clouds *clouds;
  C3D_RenderTarget *renderTargets[2];
  C3D_RenderTarget *lowerScreen;

  DVLB_s *world_dvlb = nullptr;
  DVLB_s *gui_dvlb = nullptr;

  shaderProgram_s world_shader, gui_shader;

  int world_shader_uLocProjection= 0;
  int gui_shader_uLocProjection = 0;

  C3D_AttrInfo world_vertexAttribs, gui_vertexAttribs;

  C3D_Tex logoTex;

  World *world;

  Player *player;
  WorldRenderer *worldRenderer = nullptr;
  WorkQueue *workqueue;

  ImGuiIO *io;
  bool show_demo_window;
  std::string cstyle;
  std::vector<std::string> styles;

  void RenderFrame(int eyeIndex, float iod);
  void RenderLowerScreen(DebugUI *debugUi);
  void RenderImGui();

  void RenderExpBar();
  void RenderHealth();
  void RenderGameOverlay();
  void RenderHunger();
};