#include "gui/Screen.hpp"

#include "ReCraftCore.hpp"

// TODO: Buttonlist if possible (with GUI)
Screen::Screen() {
}

Screen::~Screen() {
    // m_pClickedButton = nullptr;
    // m_buttons.clear();
}

void Screen::Init(ReCraftCore* pReCraftCore, int width, int height) {
    m_width       = width;
    m_height      = height;
    m_ReCraftCore = pReCraftCore;
    // m_pFont = pMinecraft->m_pFont;
    Init();
    UpdateTabButtonSelection();
}

void Screen::Init() {
}

void Screen::ButtonClicked(/*Button* pButton*/) {
}

void Screen::ConfirmResult(bool b, int i) {
}

bool Screen::HandleBackEvent(bool b) {
    return false;
}

bool Screen::IsPauseScreen() {
    return true;
}

bool Screen::IsErrorScreen() {
    return false;
}

bool Screen::IsInGameScreen() {
    return true;
}

void Screen::KeyPressed(int key) {
    if (key == '\x1B') { // escape
        m_ReCraftCore->SetScreen(nullptr, true);
        m_ReCraftCore->SetScreen(nullptr, false);
    }
    /*
        if (m_buttonTabList.size())
        {
    #ifndef ENH_HIGHLIGHT_BY_HOVER
            if (m_pMinecraft->m_options.m_keyBinds[Options::MENU_NEXT].value == key)
            {
                m_tabButtonIndex++;
                if (m_tabButtonIndex == int(m_buttonTabList.size()))
                    m_tabButtonIndex = 0;
            }
            if (m_pMinecraft->m_options.m_keyBinds[Options::MENU_PREVIOUS].value == key)
            {
                m_tabButtonIndex--;
                if (m_tabButtonIndex == -1)
                    m_tabButtonIndex = int(m_buttonTabList.size() - 1);
            }
            if (m_pMinecraft->m_options.m_keyBinds[Options::MENU_OK].value == key)
            {
                if (m_buttonTabList[m_tabButtonIndex]->m_bEnabled)
                {
                    m_pMinecraft->m_pSoundEngine->play("random.click");
                    buttonClicked(m_buttonTabList[m_tabButtonIndex]);
                }
            }

            updateTabButtonSelection();
    #endif
        }
    */
    /*
#ifndef ORIGINAL_CODE
    for (auto textInput : m_textInputs)
    {
        textInput->keyPressed(m_pMinecraft, key);
    }
#endif
     */
}

// TODO: TouchClick
void Screen::MouseClicked(int xPos, int yPos, int d) /* d = clicked? */ {
    /*
    if (!d) return;

    for (auto button : m_buttons)
    {
        if (button->clicked(m_pMinecraft, xPos, yPos))
        {
            m_pClickedButton = button;
            m_pMinecraft->m_pSoundEngine->play("random.click");

            */
    ButtonClicked(/*button*/); /*
}
}

#ifndef ORIGINAL_CODE
for (auto textInput : m_textInputs)
{
textInput->onClick(xPos, yPos);
}
#endif
*/
}

void Screen::MouseReleased(int xPos, int yPos, int d) {
    /*
    if (!d) return;

    if (m_pClickedButton)
    {
        m_pClickedButton->released(xPos, yPos);
        m_pClickedButton = nullptr;
    }
     */
}

void Screen::Render(int xPos, int yPos, float unused) {
    /*
    for (auto button : m_buttons)
    {
        button->render(m_pMinecraft, xPos, yPos);
    }

#ifndef ORIGINAL_CODE
    for (auto textInput : m_textInputs)
    {
        textInput->tick();
        textInput->render();
    }
#endif
     */
}

void Screen::Render3D(int xPos, int yPos, int eyeIndex, int projmat, int iod, float) {
}

void Screen::Tick() {
}

void Screen::Removed() {
}

void Screen::SetSize(int width, int height) {
    m_width  = width;
    m_height = height;
}

void Screen::UpdateEvents() {
    // if (field_10) return;
    /*
        for (int i = Mouse::_index + 1; i<int(Mouse::_inputs.size()); i++)
        {
            Mouse::_index = i;
                   TODO: TouchEvent */
    MouseEvent(); /*
}

for (int i = Keyboard::_index + 1; i<int(Keyboard::_inputs.size()); i++)
{
Keyboard::_index = i;
keyboardEvent();
}
*/
}

void Screen::KeyboardEvent() {
    // @UB: This probably behaves in an unexpected way if _inputs is empty
    /*
    #ifndef ORIGINAL_CODE
        if (Keyboard::_inputs.empty() || Keyboard::_index < 0)
            return;
    #endif

        if (Keyboard::_inputs[Keyboard::_index].field_0)
            keyPressed(Keyboard::_inputs[Keyboard::_index].field_4);
            */
}

// TODO: TouchEvent
void Screen::MouseEvent() {
    /*
    MouseInput& inp = Mouse::_inputs[Mouse::_index];

    if (1 <= inp.field_0 && inp.field_0 <= 2)
    {
        if (inp.field_4 == 1)
        TODO: TOUCHPRESSED    */
    MouseClicked(
        0, 0,
        0 /*m_width * Mouse::_x / Minecraft::width, m_height * Mouse::_y / Minecraft::height - 1, inp.field_0*/); /*
else
TODO: TOUCHRELEASED    mouseReleased(m_width * Mouse::_x / Minecraft::width, m_height * Mouse::_y / Minecraft::height -
1, inp.field_0);
}
*/
}

void Screen::RenderBackground(int unk) {
    /*
    if (m_pMinecraft->isLevelGenerated())
    {
        fillGradient(0, 0, m_width, m_height, 0xC0101010, 0xD0101010);
    }
    else
    {
        renderDirtBackground(unk);
    }
     */
}

void Screen::RenderBackground() {
    RenderBackground(0);
}

void Screen::RenderDirtBackground(int unk) {
    /*
    glDisable(GL_FOG);

    m_pMinecraft->m_pTextures->loadAndBindTexture("gui/background.png");
    glColor4f(1, 1, 1, 1);

    Tesselator& t = Tesselator::instance;
    t.begin();
    t.color(0x404040);
    t.vertexUV(0.0f,           float(m_height), 0, 0,                                   float(unk) + float(m_height)
    / 32.0f); t.vertexUV(float(m_width), float(m_height), 0, float(unk) + float(m_width) / 32.0f, float(unk) +
    float(m_height) / 32.0f); t.vertexUV(float(m_width), 0,               0, float(unk) + float(m_width) / 32.0f, 0);
    t.vertexUV(0.0f,           0,               0, 0,                                   0);
    t.draw();
     */
}


void Screen::UpdateTabButtonSelection() {
    /*
#ifndef ENH_HIGHLIGHT_BY_HOVER
    for (int i = 0; i < int(m_buttonTabList.size()); i++)
    {
        m_buttonTabList[i]->field_36 = m_tabButtonIndex == i;
    }
#endif
     */
}
