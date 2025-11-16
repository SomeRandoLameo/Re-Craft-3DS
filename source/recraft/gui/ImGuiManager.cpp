#include "gui/ImGuiManager.h"

ImGuiManager* ImGuiManager::instance = nullptr;

ImGuiManager* ImGuiManager::GetInstance() {
    if (!instance) {
        instance = new ImGuiManager();
    }
    return instance;
}

void ImGuiManager::Initialize() {
    if (initialized) return;

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 0.9f;
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(0.5f);
    io.IniFilename = nullptr;

    ImGui_ImplCtr_Init();
    ImGui_ImplCitro3D_Init();

    initialized = true;
}

void ImGuiManager::Shutdown() {
    if (!initialized) return;

    callbacks.clear();

    ImGui_ImplCitro3D_Shutdown();
    ImGui_ImplCtr_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
}

void ImGuiManager::BeginFrame() {
    if (!initialized) return;

    ImGui_ImplCitro3D_NewFrame();
    ImGui_ImplCtr_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::EndFrame(void* topTarget, void* bottomTarget) {
    if (!initialized) return;

    ImGui::Render();
    ImGui_ImplCitro3D_RenderDrawData(ImGui::GetDrawData(), topTarget, bottomTarget);
}

void ImGuiManager::RegisterCallback(const std::string& name, ImGuiCallback callback) {
    callbacks[name] = callback;
}

void ImGuiManager::UnregisterCallback(const std::string& name) {
    callbacks.erase(name);
}

void ImGuiManager::ExecuteCallbacks() {
    if (!initialized) return;

    for (auto& [name, callback] : callbacks) {
        if (callback) {
            callback();
        }
    }
}