#include "gui/screens/DeleteWorldScreen.hpp"

#include <sys/dirent.h>
#include "world/CT_World.hpp"


DeleteWorldScreen::DeleteWorldScreen(const WorldInfo& info) :
    ConfirmScreen(nullptr, "Are you sure you want to delete this world?",
                  "'" + std::string(info.name) + "' will be lost forever!", "Delete", "Cancel", 0),
    m_info(info) {
}

void DeleteWorldScreen::PostResult(bool b) {
    if (b) {
        char buffer[512];
        sprintf(buffer, "sdmc:/" SAVE_DIR "/saves/%s", m_info.path);
        DeleteWorld(buffer);
    }

    m_ReCraftCore->SetScreen(new SelectWorldBotScreen, false);
}


void DeleteWorldScreen::DeleteWorld(const char* path) {
    DIR*           dir = opendir(path);
    struct dirent* entry;

    int pathLen = strlen(path);

    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".")) {
            continue;
        }

        int entryLen = strlen(entry->d_name);

        char buffer[pathLen + entryLen + 1];
        sprintf(buffer, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            DeleteWorld(buffer);
        } else {
            unlink(buffer);
        }
    }

    closedir(dir);

    rmdir(path);
}
