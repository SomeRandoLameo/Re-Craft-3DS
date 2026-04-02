#include <sys/dirent.h>

#include "client/gui/DeleteWorldScreen.hpp"

DeleteWorldScreen::DeleteWorldScreen(const WorldInfo& info) :
    ConfirmScreen(nullptr, "Are you sure you want to delete this world?",
                  "'" + std::string(info.name) + "' will be lost forever!", "Delete", "Cancel", 0),
    m_info(info) {}

void DeleteWorldScreen::PostResult(bool b) {
    if (b) {
        DeleteWorld(ReCraftCore::GetRootDir() + "/saves/" + std::string(m_info.path));
    }

    m_ReCraftCore->SetScreen(new SelectWorldBotScreen, false);
}

void DeleteWorldScreen::DeleteWorld(const std::string& path) {
    //TODO: Freezes the 3DS
    std::error_code e; // just ignore errors
    std::filesystem::remove_all(path, e);
}
