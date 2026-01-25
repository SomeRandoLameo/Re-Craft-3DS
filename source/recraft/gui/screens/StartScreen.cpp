#include "gui/screens/StartScreen.hpp"

#include "gui/ScreenFwd.hpp"
#include "gui/screens/SelectWorldScreen.hpp"
#include "rendering/Clouds.hpp"

void StartTopScreen::Init() {
    m_clouds = new Clouds();
    Texture_Load(&m_logoTex, "romfs:/assets/textures/gui/title/craftus.png");
    // Screen::init();
}

void StartTopScreen::Removed() {
    C3D_TexDelete(&m_logoTex);
    delete m_clouds;
}

bool StartTopScreen::IsInGameScreen() {
    return false;
}

void StartTopScreen::Render3D(int mouseX, int mouseY, int eyeIndex, int projMat, int iod, float delta) {
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


void StartBotScreen::Init() {
    Screen::Init();
}

void StartBotScreen::Removed() {
    Screen::Removed();
}

bool StartBotScreen::IsInGameScreen() {
    return false;
}

void StartBotScreen::Render(int mouseX, int mouseY, float delta) {
    SpriteBatch_SetScale(2);

    SpriteBatch_BindGuiTexture(GuiTexture::MenuBackground);
    for (int i = 0; i < 160 / 32 + 1; i++) {
        for (int j = 0; j < 120 / 32 + 1; j++) {

            SpriteBatch_PushQuadColor(i * 32, j * 32, -10, 32, 32, 0, 0, 32, 32, SHADER_RGB(12, 12, 12));
        }
    }

    Gui::Offset(0, 35);
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 1);
    m_clicked_singleplayer = Gui::Button(1.f, "Singleplayer");
    Gui::EndRow();
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 1);
    m_clicked_multiplayer = Gui::Button(1.f, "Multiplayer");
    Gui::EndRow();
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 2);
    m_clicked_options = Gui::Button(0.5f, "Options");
    m_clicked_quit    = Gui::Button(0.5f, "Quit");
    Gui::EndRow();
    Screen::Render(mouseX, mouseY, delta);
}

void StartBotScreen::ButtonClicked() {
    if (m_clicked_singleplayer) {
        m_clicked_singleplayer = false;
        m_ReCraftCore->SetScreen(new SelectWorldBotScreen(), false);
    }

    if (m_clicked_multiplayer) {
        m_clicked_multiplayer = false;
        m_ReCraftCore->InitMultiPlayer();
    }

    if (m_clicked_options) {
        exit(0);
    }

    if (m_clicked_quit) {
        exit(0);
    }

    Screen::ButtonClicked();
}
