#pragma once

#include "SelectWorldScreen.hpp"
#include "gui/Screen.hpp"

class CreateWorldBotScreen : public Screen {
public:
    CreateWorldBotScreen(std::vector<WorldInfo>);

    void Init() override;
    void ButtonClicked() override;
    void Render(int mouseX, int mouseY, float f) override;

private:
    bool m_confirmed_world_options = false;
    bool m_canceled_world_options = false;

    std::vector<WorldInfo> m_worlds;

    char m_out_worldpath[256];
    char m_out_name[World::NameSize] = {'\0'};
    WorldGenType m_worldType;
    bool m_newWorld = false;

    WorldGenType m_worldGenType = WorldGenType::SuperFlat;

    Gamemode m_gamemode = Gamemode::Survival;

    static constexpr std::array<const char*, 4> m_gamemodestr = {"Survival", "Creative", "Adventure", "Spectator"};
    static constexpr std::array<const char*, 3> m_worldGenTypesStr = {"Smea", "Superflat", "Empty"};
};
