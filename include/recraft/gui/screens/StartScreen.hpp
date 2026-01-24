#pragma once

#include "gui/Screen.hpp"

class StartTopScreen : public Screen {
public:
    void Init() override;
    void Removed() override;

    bool IsInGameScreen() override;
    void Render3D(int mouseX, int mouseY, int eyeIndex, int projmat, int iod, float delta) override;
private:
    Clouds* m_clouds = nullptr;
    C3D_Tex m_logoTex;
};

class StartBotScreen : public Screen {
    void Init() override;
    void Removed() override;

    bool IsInGameScreen() override;
    void Render(int mouseX, int mouseY, float delta) override;
    void ButtonClicked() override;
private:
    bool m_clicked_singleplayer = false;
    bool m_clicked_multiplayer = false;
    bool m_clicked_options = false;
    bool m_clicked_quit = false;
};