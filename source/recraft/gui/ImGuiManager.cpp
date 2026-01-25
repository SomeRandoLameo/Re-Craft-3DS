#include "gui/ImGuiManager.hpp"

ImGuiManager* ImGuiManager::m_instance = nullptr;

ImGuiManager* ImGuiManager::GetInstance() {
    if (!m_instance) {
        m_instance = new ImGuiManager();
    }
    return m_instance;
}

void ImGuiManager::Initialize() {
    if (m_initialized) {
        return;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io        = ImGui::GetIO();
    io.FontGlobalScale = 0.9f;
    ImGuiStyle& style  = ImGui::GetStyle();
    style.ScaleAllSizes(0.5f);
    io.IniFilename = nullptr;
#ifdef IMGUI_HAS_DOCK
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    ImGui_ImplCtr_Init();
    ImGui_ImplCitro3D_Init();

    m_initialized = true;
}

void ImGuiManager::Shutdown() {
    if (!m_initialized) {
        return;
    }

    m_callbacks.clear();

    ImGui_ImplCitro3D_Shutdown();
    ImGui_ImplCtr_Shutdown();
    ImGui::DestroyContext();

    m_initialized = false;
}

void ImGuiManager::BeginFrame() {
    if (!m_initialized) {
        return;
    }

    ImGui_ImplCitro3D_NewFrame();
    ImGui_ImplCtr_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::EndFrame(void* topTarget, void* bottomTarget) {
    if (!m_initialized) {
        return;
    }

    ImGui::Render();
    ImGui_ImplCitro3D_RenderDrawData(ImGui::GetDrawData(), topTarget, bottomTarget);
}

void ImGuiManager::RegisterCallback(const std::string& name, ImGuiCallback callback) {
    m_callbacks[name] = callback;
}

void ImGuiManager::UnregisterCallback(const std::string& name) {
    m_callbacks.erase(name);
}

void ImGuiManager::ExecuteCallbacks() {
    if (!m_initialized) {
        return;
    }

    for (auto& [name, callback] : m_callbacks) {
        if (callback) {
            callback();
        }
    }
}
