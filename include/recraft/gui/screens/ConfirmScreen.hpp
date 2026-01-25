#pragma once

#include "gui/Screen.hpp"
// TODO: THIS
class ConfirmScreen : public Screen {
public:
    ConfirmScreen(Screen* pScreen, const std::string& line1, const std::string& line2, int x);
    ConfirmScreen(Screen* pScreen, const std::string& line1, const std::string& line2, const std::string& ok,
                  const std::string& cancel, int x);

    void ButtonClicked() override;
    bool HandleBackEvent(bool b) override;
    void Render(int mouseX, int mouseY, float delta) override;

    virtual void PostResult(bool b);

private:
    Screen*     m_pScreen = nullptr;
    int         field_40  = 0;
    std::string m_textLine1;
    std::string m_textLine2;
    std::string m_buttonConfirm;
    std::string m_buttonDeny;

    bool m_confirmed_deletion = false;
    bool m_canceled_deletion  = false;
};
