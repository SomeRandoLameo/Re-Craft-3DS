#include "gui/screens/SelectWorldScreen.hpp"
#include "gui/SpriteBatch.hpp"
#include "gui/screens/CreateWorldScreen.h"
#include "mpack.h"

#include <climits>
#include <sys/dirent.h>

//maybe this is even titlescreen stuff, who knows
void SelectWorldTopScreen::Init() {
    m_clouds = new Clouds();
    Texture_Load(&m_logoTex, "romfs:/assets/textures/gui/title/craftus.png");
    //Screen::init();
}

void SelectWorldTopScreen::Removed() {
    C3D_TexDelete(&m_logoTex);
    delete m_clouds;
}

bool SelectWorldTopScreen::IsInGameScreen() {
    return true;
}

void SelectWorldTopScreen::Render3D(int mouseX, int mouseY, int eyeIndex, int projMat, int iod, float delta) {
    C3D_Mtx projection;
    Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(90.f), ((400.f) / (240.f)), 0.22f, 4.f * Chunk::Size,
                        !eyeIndex ? -iod : iod, 3.f, false);

    C3D_Mtx view;
    Mtx_Identity(&view);
    Mtx_Translate(&view, 0.f, -70.f, 0.f, false);

    Mtx_RotateX(&view, -C3D_AngleFromDegrees(30.f), true);

    C3D_Mtx vp;
    Mtx_Multiply(&vp, &projection, &view);

    m_clouds->Draw(projMat, &vp, 0.f, 0.f);

    SpriteBatch_BindTexture(&m_logoTex);

    SpriteBatch_SetScale(2);
    SpriteBatch_PushQuad(100 / 2 - 76 / 2, 120 / 2, 0, 256, 64, 0, 0, 128, 32);

    SpriteBatch_PushText(0, 0, 0, INT16_MAX, true, INT_MAX, NULL, GIT_COMMIT "-" GIT_BRANCH);
    Screen::Render(mouseX, mouseY, delta);
}









void SelectWorldBotScreen::Removed() {
    m_worlds.clear();
}

void SelectWorldBotScreen::Init() {
    m_worlds.clear();

    LoadLevelSource();
}

bool SelectWorldBotScreen::IsInGameScreen() {
    return true;
}

//TODO: Separate Menu States as custom views
void SelectWorldBotScreen::Render(int mouseX, int mouseY, float delta) {
    SpriteBatch_SetScale(2);

    SpriteBatch_BindGuiTexture(GuiTexture::MenuBackground);
    for (int i = 0; i < 160 / 32 + 1; i++) {
        for (int j = 0; j < 120 / 32 + 1; j++) {
            bool overlay = j >= 2 && m_menustate == MenuState::WorldSelect;
            SpriteBatch_PushQuadColor(i * 32, j * 32, overlay ? -4 : -10, 32, 32, 0, 0, 32, 32,
                                      overlay ? INT16_MAX : SHADER_RGB(12, 12, 12));
        }
    }

    if (m_menustate == MenuState::WorldSelect) {
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
                SpriteBatch_PushSingleColorQuad(10, y - 3, -7, 140, 1, SHADER_RGB(20, 20, 20));
                SpriteBatch_PushSingleColorQuad(10, y + SpriteBatch::CharHeight + 2, -7, 140, 1, SHADER_RGB(20, 20, 20));
                SpriteBatch_PushSingleColorQuad(10, y - 3, -7, 1, SpriteBatch::CharHeight + 6, SHADER_RGB(20, 20, 20));
                SpriteBatch_PushSingleColorQuad(10 + 140, y - 3, -7, 1, SpriteBatch::CharHeight + 6, SHADER_RGB(20, 20, 20));
            }
            if (Gui::EnteredCursorInside(10, y - 3, 140, SpriteBatch::CharHeight + 6) && y < 32 * 2) {
                m_selectedWorld = (int)i;
            }
            SpriteBatch_PushText(20, y, -6, INT16_MAX, true, INT_MAX, nullptr, "%s", info.name, mouseY);
        }


        Gui::Offset(0, 2 * 32 + 5 + Gui::BUTTON_TEXT_PADDING);
        Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 1);
        m_clicked_play = Gui::Button(1.f, "Play selected world");
        Gui::EndRow();
        Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 2);
        m_clicked_new_world = Gui::Button(0.333f, "New Wrld");
        m_clicked_delete_world = Gui::Button(0.333f, "Del Wrld");
        m_clicked_mp_connect = Gui::Button(0.333f, "MP CON");
        Gui::EndRow();
        //TODO: Move somewhere separately
    } else if (m_menustate == MenuState::ConfirmDeletion) {
        Gui::Offset(0, 10);
        Gui::BeginRow(SpriteBatch_GetWidth(), 1);
        Gui::Label(0.f, true, INT16_MAX, true, "Are you sure?");
        Gui::EndRow();
        Gui::VerticalSpace(Gui::RelativeHeight(0.4f));
        Gui::BeginRowCenter(Gui::RelativeWidth(0.8f), 3);
        m_canceled_deletion = Gui::Button(0.4f, "No");
        Gui::Space(0.2f);
        m_confirmed_deletion = Gui::Button(0.4f, "Yes");
        Gui::EndRow();
    }

    Screen::Render(mouseX, mouseY, delta);
}

void SelectWorldBotScreen::ButtonClicked() {

    if (m_clicked_new_world) {
        m_clicked_new_world = false;
        m_ReCraftCore->SetScreen(new CreateWorldBotScreen(m_worlds), false);
    }
    if(m_clicked_mp_connect){
        m_clicked_mp_connect = false;
        m_newWorld = false;
        m_menustate = MenuState::WorldSelect;
        ReCraftCore::GetInstance()->InitMultiPlayer();
        //Crazy shit happens here :D
    }
    if (m_clicked_play && m_selectedWorld != -1) {
        m_clicked_play = false;
        strcpy(m_out_name, m_worlds[m_selectedWorld].name);
        strcpy(m_out_worldpath, m_worlds[m_selectedWorld].path);

        m_newWorld = false;
        m_menustate = MenuState::WorldSelect;
        ReCraftCore::GetInstance()->InitSinglePlayer(m_out_worldpath, m_out_name, &m_worldType, Gamemode::Gamemode_Count, m_newWorld);
    }
    if (m_clicked_delete_world && m_selectedWorld != -1) {
        m_clicked_delete_world = false;
        m_menustate = MenuState::ConfirmDeletion;
    }
    if (m_confirmed_deletion) {
        m_confirmed_deletion = false;
        char buffer[512];
        sprintf(buffer, "sdmc:/" SAVE_DIR "/saves/%s", m_worlds[m_selectedWorld].path);
        DeleteWorld(buffer);

        LoadLevelSource();
        m_menustate = MenuState::WorldSelect;
    }
    if (m_canceled_deletion) {
        m_canceled_deletion = false;
        m_menustate = MenuState::WorldSelect;
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

void SelectWorldBotScreen::DeleteWorld(const char* path) {
    DIR* dir = opendir(path);
    struct dirent* entry;

    int pathLen = strlen(path);

    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".")) continue;

        int entryLen = strlen(entry->d_name);

        char buffer[pathLen + entryLen + 1];
        sprintf(buffer, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR)
            DeleteWorld(buffer);
        else
            unlink(buffer);
    }

    closedir(dir);

    rmdir(path);
}