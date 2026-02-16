#include "gui/screens/SelectWorldScreen.hpp"
#include "gui/SpriteBatch.hpp"
#include "gui/screens/CreateWorldScreen.hpp"
#include "gui/screens/DeleteWorldScreen.hpp"
#include "gui/screens/StartScreen.hpp"
#include "mpack.h"

#include <climits>
#include <sys/dirent.h>

void SelectWorldBotScreen::Removed() {
    m_worlds.clear();
}

void SelectWorldBotScreen::Init() {
    m_worlds.clear();

    LoadLevelSource();
}

bool SelectWorldBotScreen::IsInGameScreen() {
    return false;
}

//TODO: Fix Selection Outline
void SelectWorldBotScreen::Render(int mouseX, int mouseY, float delta) {

    Gui::DrawDefaultBackground();


    if (Gui::IsCursorInside(0, 0, 160, 2 * 32)) {
        m_velocity += mouseX * .5f;
        m_velocity = CLAMP(m_velocity, -m_maxVelocity, m_maxVelocity);
    }
    m_scroll += m_velocity;
    m_velocity *= 0.75f;
    if (ABS(m_velocity) < 0.001f) m_velocity = 0.f;

    int maximumSize = SpriteBatch::CharHeight * 2 * m_worlds.size();
    if (m_scroll < -maximumSize) m_scroll = -maximumSize;
    if (m_scroll > 0) m_scroll = 0;

    for (size_t i = 0; i < m_worlds.size(); i++) {
        WorldInfo& info = m_worlds[i];
        int y = i * (SpriteBatch::CharHeight + SpriteBatch::CharHeight) + 10 + m_scroll;
        if (m_selectedWorld == (int)i) {
            Amy::Color borderColor = Amy::Color(20, 20, 20, 255);

//FIX PLS
            DrawHorizontalLine(10, 150, y - 3, borderColor);
            DrawHorizontalLine(10, 150, y + CHAR_HEIGHT + 2, borderColor);
            DrawVerticalLine(10, y - 3, y + CHAR_HEIGHT + 2, borderColor);
            DrawVerticalLine(150, y - 3, y + CHAR_HEIGHT + 2, borderColor);
        }
        if (Gui::EnteredCursorInside(10, y - 3, 140, SpriteBatch::CharHeight + 6) && y < 32 * 2) {
            m_selectedWorld = (int)i;
        }
        DrawStringWithShadow(info.name, 20, y, Amy::Color(255, 255, 255, 255));
    }

    Gui::Offset(0, 2 * 32 + 5 + Gui::BUTTON_TEXT_PADDING);
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 1);
    m_clicked_play = Gui::Button(1.f, "Play selected world");
    Gui::EndRow();
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 2);
    m_clicked_new_world = Gui::Button(0.333f, "New");
    m_clicked_delete_world = Gui::Button(0.333f, "Delete");
    m_clicked_back = Gui::Button(0.333f, "Back");
    Gui::EndRow();

    Screen::Render(mouseX, mouseY, delta);
}

void SelectWorldBotScreen::ButtonClicked() {

    if (m_clicked_new_world) {
        m_clicked_new_world = false;
        m_ReCraftCore->SetScreen(new CreateWorldBotScreen(m_worlds), false);
    }
    if(m_clicked_back){
        m_clicked_back = false;

        m_ReCraftCore->SetScreen(new StartBotScreen, false);
    }
    if (m_clicked_play && m_selectedWorld != -1) {
        m_clicked_play = false;
        strcpy(m_out_name, m_worlds[m_selectedWorld].name);
        strcpy(m_out_worldpath, m_worlds[m_selectedWorld].path);

        m_ReCraftCore->InitSinglePlayer(m_out_worldpath, m_out_name, &m_worldType, Gamemode::Gamemode_Count, false);
    }
    if (m_clicked_delete_world && m_selectedWorld != -1) {
        m_clicked_delete_world = false;
        WorldInfo info(m_worlds[m_selectedWorld]);
        m_ReCraftCore->SetScreen(new DeleteWorldScreen(info), false);
    }
}

void SelectWorldBotScreen::LoadLevelSource() {
    m_worlds.clear();

    DIR* directory = opendir("sdmc:/" SAVE_DIR "/saves");

    char buffer[512];

    struct dirent* entry;

    while ((entry = readdir(directory))) {
        sprintf(buffer, "sdmc:/" SAVE_DIR "/saves/%s/level.mp", entry->d_name);
        if (access(buffer, F_OK) != -1) {
            mpack_tree_t tree;
            mpack_tree_init_file(&tree, buffer, 0);
            mpack_node_t root = mpack_tree_root(&tree);

            char name[World::NameSize];
            mpack_node_copy_utf8_cstr(mpack_node_map_cstr(root, "name"), name, World::NameSize);

            if (mpack_tree_destroy(&tree) != mpack_ok) {
                continue;
            }

            WorldInfo info;
            strcpy(info.name, name);
            info.lastPlayed = 0;
            strcpy(info.path, entry->d_name);

            m_worlds.push_back(info);
        }
    }

    closedir(directory);
}