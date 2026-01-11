#include <amethyst.hpp>

#include "ReCraftCore.h"

int main() {
    ImGui::SetAllocatorFunctions([](size_t size, void*) -> void* { return Amy::Malloc(size); },
                                 [](void* ptr, void*) { Amy::Free(ptr); });
    Amy::RegisterCxxExceptionHandler();
    ReCraftCore core;
    core.Run();
    return 0;
}
