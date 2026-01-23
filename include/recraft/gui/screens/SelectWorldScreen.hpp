#pragma once

#include "gui/Screen.hpp"

typedef struct {
    uint32_t lastPlayed;
    char name[World::NameSize];
    char path[256];
} WorldInfo;

typedef enum { WorldSelect, ConfirmDeletion } MenuState;

class SelectWorldTopScreen : public Screen {
public:
    void Init() override;
    void Removed() override;

    bool IsInGameScreen() override;
    void Render3D(int mouseX, int mouseY, int eyeIndex, int projmat, int iod, float delta) override;
private:
    Clouds* m_clouds = nullptr;
    C3D_Tex m_logoTex;
};

class SelectWorldBotScreen : public Screen {
public:
    void Init() override;
    void Removed() override;

    bool IsInGameScreen() override;
    void Render(int mouseX, int mouseY, float delta) override;
    void ButtonClicked() override;
private:
    void LoadLevelSource();
    void DeleteWorld(const char* path);

    float m_maxVelocity = 20.f;

    std::vector<WorldInfo> m_worlds;
    MenuState m_menustate = MenuState::WorldSelect; // TODO: Remove

    int m_scroll = 0;
    float m_velocity = 0.f;
    int m_selectedWorld = -1;

    bool m_clicked_play = false;
    bool m_clicked_new_world = false;
    bool m_clicked_mp_connect = false;
    bool m_clicked_delete_world = false;

    bool m_confirmed_deletion = false;
    bool m_canceled_deletion = false;

    char m_out_worldpath[256];
    char m_out_name[World::NameSize] = {'\0'};
    WorldGenType m_worldType;
    bool m_newWorld = false;

};
