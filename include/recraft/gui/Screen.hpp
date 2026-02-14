#pragma once

#include "ReCraftCore.hpp"
//TODO: Buttonlist if possible (with GUI)
//TODO: Every Screen should inherit GUI.
class Screen
{
public:
    Screen();
    virtual ~Screen();

    void Init(ReCraftCore*, int, int);
    void UpdateTabButtonSelection();
    void SetSize(int width, int height);

    virtual void Render(int, int, float);
    virtual void Render3D(int, int, int, int, int, float);
    virtual void Init();
    virtual void UpdateEvents();
    virtual void MouseEvent();
    virtual void KeyboardEvent();
    virtual bool HandleBackEvent(bool);
    virtual void Tick();
    virtual void Removed();
    virtual void RenderBackground(int);
    virtual void RenderBackground();
    virtual void RenderDirtBackground(int);
    virtual bool IsPauseScreen();
    virtual bool IsErrorScreen();
    virtual bool IsInGameScreen();
    virtual void ConfirmResult(bool, int);
    virtual void ButtonClicked(/*Button*/);
    virtual void MouseClicked(int, int, int);
    virtual void MouseReleased(int, int, int);
    virtual void KeyPressed(int);

protected:
    ReCraftCore* m_ReCraftCore;
private:
    int m_width = 1;
    int m_height = 1;
    bool field_10 = false;

    //std::vector<Button*> m_buttons;
    //std::vector<Button*> m_buttonTabList;
    int m_tabButtonIndex = 0;
    //Font* m_pFont;
    //Button* m_pClickedButton = 0;

};
