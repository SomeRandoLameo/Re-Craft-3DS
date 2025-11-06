#ifndef TERRACOTTA_GAME_WINDOW_H_
#define TERRACOTTA_GAME_WINDOW_H_

#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

namespace terra {

using MouseSetCallback = std::function<void(double, double)>;
using MouseChangeCallback = std::function<void(double, double)>;
using MouseButtonCallback = std::function<void(int, int, int)>;
using MouseScrollCallback = std::function<void(double, double)>;

class GameWindow {
public:
    GameWindow(GLFWwindow* window);

    void OnKeyChange(int key, int code, int action, int mode);
    void OnMouseMove(double x, double y);
    void OnMouseButton(int button, int action, int mods);
    void OnMouseScroll(double offset_x, double offset_y);

    void RegisterMouseSet(MouseSetCallback callback) {
        m_MouseSetCallbacks.push_back(callback);
    }

    void RegisterMouseChange(MouseChangeCallback callback) {
        m_MouseChangeCallbacks.push_back(callback);
    }

    void RegisterMouseButton(MouseButtonCallback callback) {
        m_MouseButtonCallbacks.push_back(callback);
    }

    void RegisterMouseScroll(MouseScrollCallback callback) {
        m_MouseScrollCallbacks.push_back(callback);
    }

    bool IsKeyDown(int code);
private:
    GLFWwindow* m_Window;
    std::vector<MouseSetCallback> m_MouseSetCallbacks;
    std::vector<MouseChangeCallback> m_MouseChangeCallbacks;
    std::vector<MouseButtonCallback> m_MouseButtonCallbacks;
    std::vector<MouseScrollCallback> m_MouseScrollCallbacks;

    bool m_Keys[1024];
    float m_LastMouseX;
    float m_LastMouseY;
    bool m_FirstMouse;
    bool m_UpdateMouse;
};

} // ns terra

#endif
