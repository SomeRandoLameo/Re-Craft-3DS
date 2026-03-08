#include "client/gui/GuiButton.h"

GuiButton::GuiButton(int buttonId, int x, int y, int width, int height, const std::string& buttonText) {
    id = buttonId;
    m_xPos = x;
    m_yPos = y;
    text = buttonText;
    m_width = width;
    m_height = height;
}

GuiButton::GuiButton(int buttonId, int x, int y, const std::string& buttonText) {
    id = buttonId;
    m_xPos = x;
    m_yPos = y;
    text = buttonText;
    m_width = 200;
    m_height = 24;
}

GuiButton::GuiButton(int buttonId, const std::string& buttonText) {
    id = buttonId;
    text = buttonText;
    m_width = 200;
    m_height = 24;
}

bool GuiButton::Clicked(int xPos, int yPos) {
    if (!m_bEnabled)
        return false;
    if (xPos < m_xPos)
        return false;
    if (yPos < m_yPos)
        return false;
    if (xPos >= m_xPos + m_width)
        return false;
    if (yPos >= m_yPos + m_height)
        return false;

    return true;
}

void GuiButton::Render(ReCraftCore* core, int xPos, int yPos) {
    if (!m_bVisible)
        return;

    // RenderData->
}
