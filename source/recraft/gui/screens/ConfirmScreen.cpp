#include "gui/screens/ConfirmScreen.hpp"
#include "gui/SpriteBatch.hpp"

ConfirmScreen::ConfirmScreen(Screen* pScreen, const std::string& line1, const std::string& line2, int x) :
	m_textLine1(line1),
	m_textLine2(line2),
	m_pScreen(pScreen),
    m_buttonConfirm("Yes"),
    m_buttonDeny("No"),
	field_40(x)
{
}

ConfirmScreen::ConfirmScreen(Screen* pScreen, const std::string& line1, const std::string& line2, const std::string& ok, const std::string& cancel, int x) :
	m_textLine1(line1),
	m_textLine2(line2),
	m_pScreen(pScreen),
    m_buttonConfirm(ok),
    m_buttonDeny(cancel),
	field_40(x)
{
}

void ConfirmScreen::ButtonClicked() {
    if(m_confirmed_deletion){
        m_confirmed_deletion = false;
        PostResult(true);
    }

    if(m_canceled_deletion){
        m_canceled_deletion = false;
        PostResult(false);
    }

}

bool ConfirmScreen::HandleBackEvent(bool b) {
	if (!b)
		PostResult(false);

	return true;
}

void ConfirmScreen::Render(int mouseX, int mouseY, float delta) {
    Gui::DrawDefaultBackground();

    Gui::Offset(0, 10);
    Gui::BeginRowCenter(SpriteBatch_GetWidth(), 1);
    Gui::Label(0.f, true, INT16_MAX, true, m_textLine1.c_str());
    Gui::EndRow();
    Gui::BeginRow(SpriteBatch_GetWidth(), 1);
    Gui::Label(0.f, true, INT16_MAX, true, m_textLine2.c_str());
    Gui::EndRow();
    Gui::VerticalSpace(Gui::RelativeHeight(0.4f));
    Gui::BeginRowCenter(Gui::RelativeWidth(0.8f), 3);
    m_canceled_deletion = Gui::Button(0.4f, m_buttonDeny.c_str());
    Gui::Space(0.2f);
    m_confirmed_deletion = Gui::Button(0.4f, m_buttonConfirm.c_str());
    Gui::EndRow();

	Screen::Render(mouseX, mouseY, delta);
}

void ConfirmScreen::PostResult(bool b) {
	m_pScreen->ConfirmResult(b, field_40);
}


