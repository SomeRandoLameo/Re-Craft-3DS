#pragma once

#include "ReCraftCore.hpp"

class Screen;

class Button : public Gui
{
public:
    Button(int buttonId, int x, int y, int width, int height, const std::string& buttonText);
    Button(int buttonId, int x, int y, const std::string& buttonText);
    Button(int buttonId, const std::string& buttonText);


    bool Clicked(int xPos, int yPos);
    void Render(ReCraftCore*, int xPos, int yPos);

public:
    int m_width = 0;
    int m_height = 0;
    int m_xPos = 0;
    int m_yPos = 0;
    std::string text = "";
    int id;
    bool m_bEnabled = true;
    bool m_bVisible = true;
    bool field_36 = false;

#ifndef ORIGINAL_CODE
    int m_lastX = 0;
    int m_lastY = 0;
#endif
};


