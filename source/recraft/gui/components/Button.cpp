//
// Created by elias on 11.02.26.
//

#include "gui/components/Button.h"

Button::Button(int buttonId, int x, int y, int width, int height, const std::string& buttonText) {
    id = buttonId;
    m_xPos = x;
    m_yPos = y;
    text = buttonText;
    m_width = width;
    m_height = height;
}

Button::Button(int buttonId, int x, int y, const std::string& buttonText) {
    id = buttonId;
    m_xPos = x;
    m_yPos = y;
    text = buttonText;
    m_width = 200;
    m_height = 24;
}

Button::Button(int buttonId, const std::string& buttonText) {
    id = buttonId;
    text = buttonText;
    m_width = 200;
    m_height = 24;
}

bool Button::Clicked(int xPos, int yPos) {
    if (!m_bEnabled) return false;
    if (xPos < m_xPos) return false;
    if (yPos < m_yPos) return false;
    if (xPos >= m_xPos + m_width) return false;
    if (yPos >= m_yPos + m_height) return false;

    return true;
}

void Button::Render(ReCraftCore* core, int xPos, int yPos) {
    if (!m_bVisible) return;

    //RenderData->
}
