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

    bool IsInitialized() const { return m_initialized; }

private:
    ImGuiManager() {}
    ~ImGuiManager() {}

    static ImGuiManager* m_instance;
    bool m_initialized = false;
    std::map<std::string, ImGuiCallback> m_callbacks;
};