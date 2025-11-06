#include "GameWindow.h"
#include <iostream>
#include <cstring>

namespace terra {

GameWindow::GameWindow(GLFWwindow* window) : m_Window(window), m_UpdateMouse(true), m_FirstMouse(true) {
    memset(m_Keys, 0, sizeof(m_Keys));
}

bool GameWindow::IsKeyDown(int code) {
    return m_Keys[code];
}

void GameWindow::OnKeyChange(int key, int code, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        m_UpdateMouse = !m_UpdateMouse;

        if (m_UpdateMouse) {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if ((key == GLFW_KEY_3 || key == GLFW_KEY_T) && action == GLFW_PRESS) {
        m_UpdateMouse = false;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        m_UpdateMouse = true;
        m_FirstMouse = true;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (action == GLFW_PRESS && m_UpdateMouse) {
        m_Keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        m_Keys[key] = false;
    }
}

void GameWindow::OnMouseMove(double x, double y) {
    if (!m_UpdateMouse) return;

    for (auto&& cb : m_MouseSetCallbacks) {
        cb(x, y);
    }

    if (m_FirstMouse) {
        m_LastMouseX = (float)x;
        m_LastMouseY = (float)y;
        m_FirstMouse = false;
        return;
    }

    float offset_x = ((float)x - m_LastMouseX);
    float offset_y = (m_LastMouseY - (float)y);

    m_LastMouseX = (float)x;
    m_LastMouseY = (float)y;

    for (auto&& cb : m_MouseChangeCallbacks) {
        cb(offset_x, offset_y);
    }
}

void GameWindow::OnMouseButton(int button, int action, int mods) {
    if (!m_UpdateMouse) return;

    for (auto&& cb : m_MouseButtonCallbacks) {
        cb(button, action, mods);
    }
}

void GameWindow::OnMouseScroll(double offset_x, double offset_y) {
    if (!m_UpdateMouse) return;

    for (auto&& cb : m_MouseScrollCallbacks) {
        cb(offset_x, offset_y);
    }
}

} // ns terra
