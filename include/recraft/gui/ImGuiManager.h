#pragma once

#include "imgui.h"
#include "imgui_impl_citro3d.h"
#include "imgui_impl_ctr.h"
#include <citro3d.h>
#include <functional>
#include <map>
#include <string>

using ImGuiCallback = std::function<void()>;

class ImGuiManager {
public:
    static ImGuiManager* GetInstance();

    void Initialize();
    void Shutdown();
    void BeginFrame();
    void EndFrame(void* topTarget, void* bottomTarget);

    void RegisterCallback(const std::string& name, ImGuiCallback callback);

    void UnregisterCallback(const std::string& name);

    void ExecuteCallbacks();

    bool IsInitialized() const { return initialized; }

private:
    ImGuiManager() : initialized(false) {}
    ~ImGuiManager() {}

    static ImGuiManager* instance;
    bool initialized;
    std::map<std::string, ImGuiCallback> callbacks;
};