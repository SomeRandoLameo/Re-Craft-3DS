#include "gui/screens/StartScreen.hpp"
#include "gui/SpriteBatch.hpp"
#include "gui/screens/SelectWorldScreen.hpp"

void StartTopScreen::Init() {
    m_clouds = new Clouds();
    m_ReCraftCore->GetAssetManager()->AutoLoad("logoTex", "romfs:/assets/textures/gui/title/craftus.png");
}

void StartTopScreen::Removed() {
    C3D_TexDelete(&m_logoTex);
    delete m_clouds;
}

bool StartTopScreen::IsInGameScreen() {
    return false;
}

void StartTopScreen::Render(int mouseX, int mouseY, float delta) {

    //TODO: Why does this tile???
    RenderData->DrawTex(ReCraftCore::GetInstance()->GetAssetManager()->Get<Amy::Texture>("logoTex"));
    DrawTexturedModalRect(18,88, 0, 0,  128, 32);

    DrawStringWithShadow(GIT_COMMIT "-" GIT_BRANCH,0,0, Amy::Color(255, 255, 255, 255));
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
    Gui::DrawDefaultBackground();

    Gui::Offset(0, 35);
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 1);
    m_clicked_singleplayer = Gui::Button(1.f, "Singleplayer");
    Gui::EndRow();
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 1);
    m_clicked_multiplayer = Gui::Button(1.f, "Multiplayer");
    Gui::EndRow();
    Gui::BeginRowCenter(Gui::RelativeWidth(0.95f), 2);
    m_clicked_options = Gui::Button(0.5f, "Options");
    m_clicked_quit = Gui::Button(0.5f, "Quit");
    Gui::EndRow();
    Screen::Render(mouseX, mouseY, delta);
}

void StartBotScreen::ButtonClicked() {
    if(m_clicked_singleplayer){
        m_clicked_singleplayer = false;
        m_ReCraftCore->SetScreen(new SelectWorldBotScreen(), false);
    }

    if(m_clicked_multiplayer){
        m_clicked_multiplayer = false;
        m_ReCraftCore->InitMultiPlayer();
    }

    if(m_clicked_options){
        exit(0);
    }

    if(m_clicked_quit){
        exit(0);
    }

    Screen::ButtonClicked();
}
