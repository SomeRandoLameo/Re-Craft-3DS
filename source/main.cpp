#include <amethyst.hpp>

#include "ReCraftCore.hpp"

int main() {
    Amy::RegisterCxxExceptionHandler();
    RC::InitLog();
    ReCraftCore core;
    core.Run();
    RC::DeinitLog();
    return 0;
}
