#pragma once

#include "ConfirmScreen.hpp"
#include "gui/screens/SelectWorldScreen.hpp"

class DeleteWorldScreen : public ConfirmScreen {
public:
    DeleteWorldScreen(const WorldInfo& info);

    void PostResult(bool b) override;

private:
    WorldInfo m_info;
    void      DeleteWorld(const char* path);
};
