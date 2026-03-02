#pragma once

#include <cstdlib>
#include <format>
#include <iostream>
#include <misc/Formatters.hpp>
#include <string>

namespace RC {
    void InitLog();
    void DeinitLog();
    void WriteCrash(const std::string& txt);
    void CrashScreen(const std::string& txt);
    void Log(const std::string& txt);
    template <typename... Args>
    void Log(std::format_string<Args...> fmt, Args&&... args) {
        std::string msg = std::format(fmt, std::forward<Args>(args)...);
        Log(msg);
    }
} // namespace RC

template <typename... Args>
[[noreturn]]
void Crash(std::format_string<Args...> fmt, Args&&... args) {
    std::string msg = std::format(fmt, std::forward<Args>(args)...);
    RC::WriteCrash(msg);
    RC::CrashScreen(msg);
    std::cout << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

#ifdef RC_LOGGING
#define LOG(fmt, ...) RC::Log("[{}:{}]: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif
