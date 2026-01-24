#include "gui/screens/CreateWorldScreen.hpp"
#include "gui/SpriteBatch.hpp"

#include <utility>

CreateWorldBotScreen::CreateWorldBotScreen(std::vector<WorldInfo> worldInfo) {
    m_worlds = std::move(worldInfo);
}


void CreateWorldBotScreen::Init() {
    Screen::Init();
}
//TODO: De-Noodle-c-ify
void CreateWorldBotScreen::Render(int mouseX, int mouseY, float f) {
    SpriteBatch_SetScale(2);

    SpriteBatch_BindGuiTexture(GuiTexture::MenuBackground);
    for (int i = 0; i < 160 / 32 + 1; i++) {
        for (int j = 0; j < 120 / 32 + 1; j++) {

            SpriteBatch_PushQuadColor(i * 32, j * 32,  -10, 32, 32, 0, 0, 32, 32,
                                       SHADER_RGB(12, 12, 12));
        }
    }

    Gui::Offset(0, 10);

    // TODO: This is terrible
    Gui::BeginRowCenter(Gui::RelativeWidth(0.9f), 3);
    Gui::Label(0.45f, true, INT16_MAX, false, "Game Mode:");
    Gui::Space(0.1f);
    if (Gui::Button(0.45f, "%s", m_gamemodestr[m_gamemode])) {
        m_gamemode = static_cast<Gamemode>(static_cast<int>(m_gamemode) + 1);
        if (m_gamemode == Gamemode_Count)
            m_gamemode = static_cast<Gamemode>(0);
    }
    Gui::EndRow();
    // TODO: This is terrible as well
    Gui::BeginRowCenter(Gui::RelativeWidth(0.9f), 3);
    Gui::Label(0.45f, true, INT16_MAX, false, "World type:");
    Gui::Space(0.1f);
    if (Gui::Button(0.45f, "%s", m_worldGenTypesStr[m_worldGenType])) {
        m_worldGenType = static_cast<WorldGenType>(static_cast<int>(m_worldGenType) + 1);
        if (m_worldGenType == WorldGenTypes_Count)
            m_worldGenType = static_cast<WorldGenType>(0);
    }
    Gui::EndRow();

    Gui::VerticalSpace(Gui::RelativeHeight(0.4f));

    Gui::BeginRowCenter(Gui::RelativeWidth(0.9f), 3);
    m_canceled_world_options = Gui::Button(0.45f, "Cancel");
    Gui::Space(0.1f);
    m_confirmed_world_options = Gui::Button(0.45f, "Continue");
    Screen::Render(mouseX, mouseY, f);
}

void CreateWorldBotScreen::ButtonClicked() {
    if(m_confirmed_world_options){
        m_confirmed_world_options = false;
        m_worldType = m_worldGenType;

        static SwkbdState swkbd;
        static char name[World::NameSize];

        swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 2, World::NameSize);
        swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0,  World::NameSize);
        swkbdSetHintText(&swkbd, "Enter the world name");

        int button = swkbdInputText(&swkbd, name, 12);

        strcpy(m_out_name, name);
        if (button == SWKBD_BUTTON_CONFIRM) {
            strcpy(m_out_worldpath, m_out_name);

            int length = strlen(m_out_worldpath);

            for (int i = 0; i < length; i++) {
                if (m_out_worldpath[i] == '/' || m_out_worldpath[i] == '\\' || m_out_worldpath[i] == '?' ||
                    m_out_worldpath[i] == ':' || m_out_worldpath[i] == '|' || m_out_worldpath[i] == '<' ||
                    m_out_worldpath[i] == '>')
                    m_out_worldpath[i] = '_';
            }

            while (true) {
                bool alreadyExisting = false;
                for (const auto& info : m_worlds) {
                    if (!strcmp(m_out_worldpath, info.path)) {
                        alreadyExisting = true;
                        break;
                    }
                }
                if (!alreadyExisting) break;

                m_out_worldpath[length] = '_';
                m_out_worldpath[length + 1] = '\0';
                ++length;
            }

            m_newWorld = true;
            ReCraftCore::GetInstance()->InitSinglePlayer(m_out_worldpath, name, &m_worldType, m_gamemode, m_newWorld);

        }
    }
    if (m_canceled_world_options){
        m_ReCraftCore->SetScreen(new SelectWorldBotScreen, false);
    }
}