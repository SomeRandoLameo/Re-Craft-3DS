#include <3ds.h>

#include <ReCraftCore.hpp>
#include <fstream>
#include <misc/Crash.hpp>

namespace RC {
#ifdef RC_LOGGING
    std::ofstream m_log_file;

    void __log_kill() { m_log_file.close(); }
#endif

    void InitLog() {
#ifdef RC_LOGGING
        std::error_code e;
        std::filesystem::create_directories(ReCraftCore::GetRootDir(), e);
        if (std::filesystem::exists(ReCraftCore::GetRootDir() + "/log.txt")) {
            if (std::filesystem::exists(ReCraftCore::GetRootDir() + "/log.last.txt")) {
                std::filesystem::remove(ReCraftCore::GetRootDir() + "/log.last.txt", e);
            }
            std::filesystem::rename(ReCraftCore::GetRootDir() + "/log.txt", ReCraftCore::GetRootDir() + "/log.last.txt",
                                    e);
        }
        m_log_file.open(ReCraftCore::GetRootDir() + "/log.txt");
#endif
    }

    void DeinitLog() {
#ifdef RC_LOGGING
        __log_kill();
#endif
    }

    void WriteCrash(const std::string& msg) {
        std::ofstream off(ReCraftCore::GetRootDir() + "/crash.txt");
        if (off.is_open()) {
            off << msg;
            off.close();
        }
#ifdef RC_LOGGING
        __log_kill();
#endif
    }

    void Log(const std::string& txt) {
#ifdef RC_LOGGING
        if (m_log_file.is_open()) {
            m_log_file << txt << std::endl;
        }
#endif
    }

    void CrashScreen(const std::string& msg) {
        gfxInitDefault();
        consoleInit(GFX_TOP, nullptr);
        std::cout << "ReCraft - " GIT_BRANCH " - " GIT_COMMIT " Error Handler\n\n";
        std::cout << msg << std::endl;
        std::cout << "\nPress Start to exit...\n";
        while (aptMainLoop) {
            hidScanInput();
            if (hidKeysDown() & KEY_START) {
                break;
            }
            gspWaitForVBlank();
            gfxSwapBuffers();
        }
    }
} // namespace RC
