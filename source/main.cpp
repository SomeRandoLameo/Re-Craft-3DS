#include <amethyst.hpp>

#include "ReCraftCore.hpp"

int main() {
    Amy::RegisterCxxExceptionHandler();
    ReCraftCore core;
    core.Run();
    return 0;
}
