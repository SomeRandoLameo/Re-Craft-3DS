#pragma once

#include "gui/Screen.hpp"
#include "world/CT_World.hpp"

typedef struct {
    u32  lastPlayed;
    char name[World::NameSize];
    char path[256];
} WorldInfo;

class SelectWorldBotScreen : public Screen {
public:
    void Init() override;
    void Removed() override;

    bool IsInGameScreen() override;
    void Render(int mouseX, int mouseY, float delta) override;
    void ButtonClicked() override;

private:
    void LoadLevelSource();

    float m_maxVelocity = 20.f;

    std::vector<WorldInfo> m_worlds;

    int   m_scroll        = 0;
    float m_velocity      = 0.f;
    int   m_selectedWorld = -1;

    bool m_clicked_play         = false;
    bool m_clicked_new_world    = false;
    bool m_clicked_back         = false;
    bool m_clicked_delete_world = false;

    char         m_out_worldpath[256];
    char         m_out_name[World::NameSize] = {'\0'};
    WorldGenType m_worldType;
};
