#include "gui/screens/ConfirmScreen.hpp"
//TODO: THIS
ConfirmScreen::ConfirmScreen(Screen* pScreen, const std::string& line1, const std::string& line2, int x) :
	m_textLine1(line1),
	m_textLine2(line2),
	m_pScreen(pScreen),
	field_40(x)
{
}

ConfirmScreen::ConfirmScreen(Screen* pScreen, const std::string& line1, const std::string& line2, const std::string& ok, const std::string& cancel, int x) :
	m_textLine1(line1),
	m_textLine2(line2),
	m_pScreen(pScreen),
	field_40(x)
{
}

void ConfirmScreen::ButtonClicked()
{
	//postResult(pButton->field_30 == 0);
}

bool ConfirmScreen::HandleBackEvent(bool b)
{
	if (!b)
		PostResult(false);

	return true;
}

void ConfirmScreen::Init()
{
	//m_btnOK.m_xPos = m_width / 2 - 4 - 120;
	//m_btnCancel.m_xPos = m_width / 2 + 4;

	//m_btnCancel.m_yPos = m_btnOK.m_yPos = m_height / 6 + 72;

	//m_btnOK.m_width  = m_btnCancel.m_width = 120;
	//m_btnOK.m_height = m_btnCancel.m_height = 24;

	//m_buttons.push_back(&m_btnOK);
	//m_buttons.push_back(&m_btnCancel);
	//m_buttonTabList.push_back(&m_btnOK);
	//m_buttonTabList.push_back(&m_btnCancel);
}

void ConfirmScreen::Render(int mouseX, int mouseY, float f)
{
	//renderBackground();
	//drawCenteredString(m_pFont, m_textLine1, m_width / 2, 50, 0xFFFFFF);
	//drawCenteredString(m_pFont, m_textLine2, m_width / 2, 70, 0xFFFFFF);
	Screen::Render(mouseX, mouseY, f);
}

void ConfirmScreen::PostResult(bool b)
{
	m_pScreen->ConfirmResult(b, field_40);
}


